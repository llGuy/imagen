/*
  For the lesson0, set every pixel to red (255 for the red, and 0 for the rest)
 */

int width;
int height;

void setup(int w, int h) {
  /*
    Setup code. This is where you may allocate buffer memory, load
    images from files, or any kind of processing which needs to happen
    before the application starts.
   */

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
  /*
    Update code. This is where you update the actual values of the pixels
    which get displayed onto the window. Pixels are formatted in the following
    manner:
      - each pixel is 4 bytes long
        (1 byte for red, 1 for blue, 1 for green, then 1 for alpha)
      - they are all stored right next to eachother in a contiguous piece of
        memory at the location of the pointer: "pixels"
      - the display is "width"x"height" large
      - the bottom row of pixels is stored first, then the second from the
        bottom - you can say the pixels are stored from bottom to top,
        left to right
   */

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width/2; ++x) {
      unsigned char *pixel = getPixel(pixels, x, y, width);
      setPixel(pixel, 255, 0, 255);
    }
  }
}
