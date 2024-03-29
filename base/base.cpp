#include <base.hpp>

#include <string.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

static GLFWwindow *window;
int width, height;
static void *pixels;
vec2 mouseDelta;
vec2 previous;
bool fpsMode = false;

void     doBoringSetup();
bool32_t isWindowOpen();
void     render();
void     doBoringUnsetup();

extern void setup();
extern void update(unsigned char *pixels);

vec2 mouseMovement() {
  return mouseDelta;
}

void toggleFPSMode() {
  fpsMode ^= 1;

  if (fpsMode) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }
  else {
     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

int main(int argc, char *argv[]) {
  doBoringSetup();

  setup();

  while (isWindowOpen()) {
    render();
  }

  doBoringUnsetup();

  return 0;
}

void doBoringSetup() {
  width = 512, height = 512;

  glfwInit();
  window = glfwCreateWindow(
    width, height, "Software Rasterizer", NULL, NULL);
  glfwMakeContextCurrent(window);

  pixels = malloc(sizeof(unsigned char) * width * height * 4);

  previous = getMousePos();
}

bool32_t isWindowOpen() {
  return !glfwWindowShouldClose(window);
}

void render() {
  glfwPollEvents();

  vec2 current = getMousePos();

  mouseDelta = current - previous;
  mouseDelta.y *= -1.0f;
  previous = current;

  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  memset(pixels, 0, sizeof(u8) * width * height * 4);

  update((unsigned char *)pixels);
  glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

  glfwSwapBuffers(window);
}

void doBoringUnsetup() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

int isKeyPressedImpl(int key) {
  return glfwGetKey(window, key);
}
