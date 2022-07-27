/*
** Here, we need to add depth. We need to allocate a buffer
** which is going to hold depth information per pixel.
*/

#include <base.hpp>

float angle;
float maxDepth;

float *depthBuffer;

void setup() {
  angle = 0.0f;

  depthBuffer = (float *)malloc(sizeof(float) * width * height);
}

struct uniform {
  // Projection data
  float nearPlane;

  // Transformation data
  float scale;
  mat3 rotation;
  vec3 translation;
};

struct attribute {
  vec3 color;
};

unsigned char *getPixel(unsigned char *pixels, int x, int y) {
  return pixels + (x + y * width) * 4;
}

float *getDepth(float *depth, int x, int y) {
  return depth + x + y * width;
}

void setPixel(
  unsigned char *pixel,
  unsigned char r,
  unsigned char g,
  unsigned char b) {
  pixel[0] = r;
  pixel[1] = g;
  pixel[2] = b;
}

vec2 convertFromClipToPixel(vec2 ndc) {
  return vec2(width, height) * (ndc / 2.0f + vec2(0.5f));
}

void drawTriangle(unsigned char *pixels, vec2 *clipVerts, float *depth, attribute *attribs) {
  // Convert to pixel coordinates
  vec2 pixelVerts[3] = {
    convertFromClipToPixel(clipVerts[0]),
    convertFromClipToPixel(clipVerts[1]),
    convertFromClipToPixel(clipVerts[2])
  };

  vec2 lower = min(pixelVerts[0], min(pixelVerts[1], pixelVerts[2]));
  vec2 upper = max(pixelVerts[0], max(pixelVerts[1], pixelVerts[2]));

  // Make sure to clamp these to the min/max of the window itself
  lower = clamp(lower, vec2(0), vec2(width, height));
  upper = clamp(upper, vec2(0), vec2(width, height));

  vec2 u = pixelVerts[1] - pixelVerts[0];
  vec2 v = pixelVerts[2] - pixelVerts[0];

  vec2 w1 = u;
  vec2 w2 = v - proj(v, u);

  for (int y = lower.y; y < upper.y; ++y) {
    for (int x = lower.x; x < upper.x; ++x) {

      vec2 point = vec2(x, y);
      vec2 delta = point - pixelVerts[0];

      float gamma = dot(delta, w2) / dot(w2, w2);
      float beta = ( dot(delta, w1) / dot(w1, w1) ) - ( dot(delta, w2) / dot(w2, w2) ) * ( dot(u, v) / dot(u, u) );

      if (beta + gamma <= 1.0f && beta >= 0.0f && gamma >= 0.0f) {
        float alpha = 1.0f - beta - gamma;

        // We take the inverse so that 0.0f represents the furthest possible thing
        float interpolatedDepth = 1.0f/(alpha * depth[0] + beta * depth[1] + gamma * depth[2]);
        float *currentDepth = getDepth(depthBuffer, x, y);
        if (interpolatedDepth > *currentDepth) {
          attribute interpolated;
          interpolated.color = alpha * attribs[0].color + beta * attribs[1].color + gamma * attribs[2].color;

          unsigned char *pixel = getPixel(pixels, x, y);
          setPixel(pixel, interpolated.color.r, interpolated.color.g, interpolated.color.b);

          *currentDepth = interpolatedDepth;
        }
      }

    }
  }
}

vec3 transformVertex(vec3 vert, uniform *uniformData) {
  vert = uniformData->scale * (uniformData->rotation * vert) + uniformData->translation;

  vert.x *= uniformData->nearPlane / (vert.z * 0.57f);
  vert.y *= uniformData->nearPlane / (vert.z * 0.57f);

  return vert;
}

void transformAndDrawTriangle(
  unsigned char *pixels, vec3 *verts, attribute *attribs,
  uniform *uniformData) {
  vec3 a = transformVertex(verts[0], uniformData);
  vec3 b = transformVertex(verts[1], uniformData);
  vec3 c = transformVertex(verts[2], uniformData);

  vec2 transformed[3] = {vec2(a), vec2(b), vec2(c)};

  float depth[3] = {a.z, b.z, c.z};

  drawTriangle(pixels, transformed, depth, attribs);
}

void update(unsigned char *pixels) {
  memset(depthBuffer, 0, sizeof(float) * width * height);

  vec3 verts[] = {
    vec3(-0.5f, -0.5f, 0.0f),
    vec3(0.0f, 0.5f, 0.0f),
    vec3(0.5f, -0.5f, 0.0f)
  };

  { // First triangle
    attribute attribs[] = {
      { vec3(255, 0, 0) },
      { vec3(0, 255, 0) },
      { vec3(0, 0, 255) }
    };

    uniform uniformDataA = {
      1.0f,
      1.0f,
      rotate(radians(angle), vec3(0.0f, 1.0f, 0.0f)),
      vec3(0.0f, 0.0f, 2.0f)
    };

    transformAndDrawTriangle(pixels, verts, attribs, &uniformDataA);
  }

  {
    attribute attribs[] = {
      { vec3(255, 255, 0) },
      { vec3(0, 255, 255) },
      { vec3(255, 0, 255) }
    };

    uniform uniformDataA = {
      1.0f,
      2.0f,
      rotate(radians(angle * 2.0f), vec3(1.0f, 1.0f, 1.0f)),
      vec3(0.5f, 0.5f, 5.0f)
    };

    transformAndDrawTriangle(pixels, verts, attribs, &uniformDataA);
  }

  angle += 1.0f;
}
