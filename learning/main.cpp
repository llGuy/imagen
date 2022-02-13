#include <GLFW/glfw3.h>
#include <string.h>

int main() {
  int width = 300, height = 300;

  glfwInit();
  GLFWwindow *window = glfwCreateWindow(300, 300, "Hello World", NULL, NULL);
  glfwMakeContextCurrent(window);

  unsigned char *pixels = new unsigned char[width * height * 4];
  memset(pixels, 0, sizeof(unsigned char) * width * height * 4);

  for (int y = 100; y < 200; ++y) {
    int yOffset = y * (width * 4);
    int xOffset = 100 * 4;
    
    memset(pixels + yOffset + xOffset, 0xAA, 100 * 4);
  }

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
