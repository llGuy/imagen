#include <base.hpp>

int width;
int height;

int positionX;
int positionY;

const int kSize = 64;

void setup(int w, int h) {
  positionX = 128;
  positionY = 128;

  width = w;
  height = h;
}

unsigned char *getPixel(unsigned char *pixels, int x, int y, int w) {
  return pixels + (x + y * w) * 4;
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

void update(unsigned char *pixels) {
  for (int y = 0; y < kSize; ++y) {
    for (int x = 0; x < kSize; ++x) {
      unsigned char *pixel = getPixel(pixels, x + positionX, y + positionY, width);
      setPixel(pixel, 255, 0, 0);
    }
  }

  if (isKeyPressed(W)) {
    positionY += 1;
  }
  if (isKeyPressed(A)) {
    positionX -= 1;
  }
  if (isKeyPressed(S)) {
    positionY -= 1;
  }
  if (isKeyPressed(D)) {
    positionX += 1;
  }
}