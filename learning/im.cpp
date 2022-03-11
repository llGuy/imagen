#include "im.h"

#include <string.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

GLFWwindow *window;
int width, height;
void *pixels;

void     doBoringSetup();
bool32_t isWindowOpen();
void     render();
void     doBoringUnsetup();

extern void setup();
extern void update(u8 *pixels, int width, int height);

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
  width = 300, height = 300;

  glfwInit();
  window = glfwCreateWindow(
    300, 300, "Software Rasterizer", NULL, NULL);
  glfwMakeContextCurrent(window);

  pixels = malloc(sizeof(u8) * width * height * 4);
}

bool32_t isWindowOpen() {
  return !glfwWindowShouldClose(window);
}

void render() {
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  memset(pixels, 0, sizeof(u8) * width * height * 4);

  update((u8 *)pixels, width, height);
  glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

  glfwSwapBuffers(window);
}

void doBoringUnsetup() {
  glfwDestroyWindow(window);
  glfwTerminate();
}