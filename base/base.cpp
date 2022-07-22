#include "base.hpp"

#include <string.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

static GLFWwindow *window;
static int width, height;
static void *pixels;

void     doBoringSetup();
bool32_t isWindowOpen();
void     render();
void     doBoringUnsetup();

extern void setup(int width, int height);
extern void update(unsigned char *pixels);

int main(int argc, char *argv[]) {
  doBoringSetup();

  setup(width, height);

  while (isWindowOpen()) {
    render();
  }

  doBoringUnsetup();

  return 0;
}

void doBoringSetup() {
  width = 300, height = 300;

  glfwInit();
  window = glfwCreateWindow(
    300, 300, "Software Rasterizer", NULL, NULL);
  glfwMakeContextCurrent(window);

  pixels = malloc(sizeof(unsigned char) * width * height * 4);
}

bool32_t isWindowOpen() {
  return !glfwWindowShouldClose(window);
}

void render() {
  glfwPollEvents();
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
