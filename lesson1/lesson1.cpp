#include <base.hpp>

int width;
int height;

void setup(int w, int h) {
  width = w;
  height = h;
}

unsigned char *getPixel(unsigned char *pixels, int x, int y) {
  return pixels + (x + y * width) * 4;
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

void drawTriangle(unsigned char *pixels, vec2 a, vec2 b, vec2 c) {
  vec2 lower = min(a, min(b, c));
  vec2 upper = max(a, max(b, c));

  vec2 u = b - a;
  vec2 v = c - a;

  vec2 w1 = u;
  vec2 w2 = v - proj(v, u);

  for (int y = lower.y; y < upper.y; ++y) {
    for (int x = lower.x; x < upper.x; ++x) {

      vec2 point = vec2(x, y);
      vec2 delta = point - a;

      float alpha = ( dot(delta, w1) / dot(w1, w1) ) - ( dot(delta, w2) / dot(w2, w2) ) * ( dot(u, v) / dot(u, u) );
      float beta = dot(delta, w2) / dot(w2, w2);

      if (alpha + beta <= 1.0f && alpha >= 0.0f && beta >= 0.0f) {
        unsigned char *pixel = getPixel(pixels, x, y);
        setPixel(pixel, 255, 0, 0);
      }

    }
  }
}

void update(unsigned char *pixels) {
  vec2 a = {32, 32};
  vec2 b = {width/2, height-32};
  vec2 c = {width-32, 32};
  drawTriangle(pixels, a, b, c);
}
