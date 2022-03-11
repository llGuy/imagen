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

void setup() {
  
}

void update(unsigned char *pixels, int width, int height) {
  
}
