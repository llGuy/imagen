#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

static void quit(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

extern void setup(int width, int height);
extern void update(unsigned char *pixels);

#define MAX_FRAMES_IN_FLIGHT 3

int main() {
  const id<MTLDevice> gpu = MTLCreateSystemDefaultDevice();
  const id<MTLCommandQueue> queue = [gpu newCommandQueue];
  CAMetalLayer *swapchain = [CAMetalLayer layer];
  swapchain.device = gpu;
  swapchain.opaque = YES;

  int width = 640;
  int height = 480;

  setup(width, height);

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow *window =
      glfwCreateWindow(width, height, "GLFW Metal", NULL, NULL);
  NSWindow *nswindow = glfwGetCocoaWindow(window);
  nswindow.contentView.layer = swapchain;
  nswindow.contentView.wantsLayer = YES;

  glfwSetKeyCallback(window, quit);

  // Setup render context for the lessons
  int frame = 0;
  dispatch_semaphore_t semaphore =
      dispatch_semaphore_create(MAX_FRAMES_IN_FLIGHT);
  id<MTLRenderPipelineState> pso;
  id<MTLTexture> textures[MAX_FRAMES_IN_FLIGHT];
  uint8_t *pixelBuffers[MAX_FRAMES_IN_FLIGHT];

  { // Build the shaders
    const char *shaderSrc =
        "#include <metal_stdlib>\n"
        "using namespace metal;\n"
        "struct v2f\n"
        "{\n"
        "float4 position [[position]];\n"
        "half3 color;\n"
        "float2 uvs;\n"
        "};\n"
        "v2f vertex vertexMain( uint vertexId [[vertex_id]] )\n"
        "{\n"
        "float2 pos[] = { float2(-1.0), float2(-1.0, 1.0), float2(1.0, -1.0), "
        "float2(1.0, 1.0) };\n"
        "float3 colors[] = { float3(1.0, 0.0, 0.0), float3(0.0, 1.0, 0.0), "
        "float3(0.0, 0.0, 1.0), float3(1.0, 1.0, 0.0) };\n"
        "float2 uvs[] = { float2(0.0), float2(0.0, 1.0), float2(1.0, 0.0), "
        "float2(1.0) };\n"

        "v2f o;\n"
        "o.position = float4( pos[ vertexId ], 0.0, 1.0 );\n"
        "o.color = half3 ( colors[ vertexId ] );\n"
        "o.uvs = float2(uvs[vertexId]);\n"
        "return o;\n"
        "}\n"
        "half4 fragment fragmentMain( v2f in [[stage_in]],\n"
        "texture2d< half, access::sample > tex [[texture(0)]] )\n"
        "{\n"
        "constexpr sampler s( address::clamp_to_edge, filter::linear );\n"
        "half3 texel = tex.sample( s, in.uvs ).rgb;\n"
        "return half4( texel, 1.0 );\n"
        "}\n";

    NSString *src = [NSString stringWithUTF8String:shaderSrc];

    NSError *error;
    id<MTLLibrary> library = [gpu newLibraryWithSource:src
                                               options:nil
                                                 error:&error];

    if (!library) {
      [NSException raise:@"Failed to read shaders"
                  format:@"%@", [error localizedDescription]];
    }

    MTLRenderPipelineDescriptor *pDesc =
        [[MTLRenderPipelineDescriptor alloc] init];
    pDesc.vertexFunction = [library newFunctionWithName:@"vertexMain"];
    pDesc.fragmentFunction = [library newFunctionWithName:@"fragmentMain"];
    pDesc.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;

    pso = [gpu newRenderPipelineStateWithDescriptor:pDesc error:&error];

    if (!pso) {
      [NSException raise:@"Failed to create pipeline state"
                  format:@"%@", [error localizedDescription]];
    }
  }

  { // Build the textures
    int tw = width;
    int th = height;

    MTLTextureDescriptor *pTextureDesc = [[MTLTextureDescriptor alloc] init];
    pTextureDesc.width = tw;
    pTextureDesc.height = th;
    pTextureDesc.pixelFormat = MTLPixelFormatRGBA8Unorm;
    pTextureDesc.textureType = MTLTextureType2D;
    pTextureDesc.storageMode = MTLStorageModeManaged;
    pTextureDesc.usage = MTLResourceUsageSample | MTLResourceUsageRead;

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
      pixelBuffers[i] = (uint8_t *)malloc(width * height * 4);
      textures[i] = [gpu newTextureWithDescriptor:pTextureDesc];

      uint8_t *pTextureData = (uint8_t *)alloca(tw * th * 4);
      for (size_t y = 0; y < th; ++y) {
        for (size_t x = 0; x < tw; ++x) {
          bool isWhite = (x ^ y) & 0b1000000;
          uint8_t c = isWhite ? 0xFF : 0xA;

          size_t i = y * tw + x;

          pTextureData[i * 4 + 0] = c;
          pTextureData[i * 4 + 1] = c;
          pTextureData[i * 4 + 2] = c;
          pTextureData[i * 4 + 3] = 0xFF;
        }
      }

      MTLRegion region = MTLRegionMake3D(0, 0, 0, tw, th, 1);

      [textures[i] replaceRegion:region
                     mipmapLevel:0
                       withBytes:pTextureData
                     bytesPerRow:tw * 4];
    }
  }

  MTLClearColor color = MTLClearColorMake(0, 0, 0, 1);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    frame = (frame + 1) % MAX_FRAMES_IN_FLIGHT;

    memset(pixelBuffers[frame], 0, width * height * 4);
    update(pixelBuffers[frame]);

    MTLRegion region = MTLRegionMake3D(0, 0, 0, width, height, 1);
    [textures[frame] replaceRegion:region
                       mipmapLevel:0
                         withBytes:pixelBuffers[frame]
                       bytesPerRow:width * 4];

    @autoreleasepool {
      color.red = (color.red > 1.0) ? 0 : color.red + 0.01;

      id<CAMetalDrawable> surface = [swapchain nextDrawable];

      MTLRenderPassDescriptor *pass =
          [MTLRenderPassDescriptor renderPassDescriptor];
      pass.colorAttachments[0].clearColor = color;
      pass.colorAttachments[0].loadAction = MTLLoadActionClear;
      pass.colorAttachments[0].storeAction = MTLStoreActionStore;
      pass.colorAttachments[0].texture = surface.texture;

      id<MTLCommandBuffer> buffer = [queue commandBuffer];
      dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
      [buffer addCompletedHandler:^void(id<MTLCommandBuffer> buffer) {
        dispatch_semaphore_signal(semaphore);
      }];

      id<MTLRenderCommandEncoder> encoder =
          [buffer renderCommandEncoderWithDescriptor:pass];

      [encoder setRenderPipelineState:pso];
      [encoder setFragmentTexture:textures[frame] atIndex:0];
      [encoder drawPrimitives:MTLPrimitiveTypeTriangleStrip
                  vertexStart:0
                  vertexCount:4];

      [encoder endEncoding];
      [buffer presentDrawable:surface];
      [buffer commit];
    }
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
