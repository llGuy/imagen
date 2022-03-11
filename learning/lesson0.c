#define FINAL

/*
  For the lesson0, set every pixel to red (255 for the red, and 0 for the rest)
 */

unsigned char currentG;

void setup() {
  /* 
    Setup code. This is where you may allocate buffer memory, load
    images from files, or any kind of processing which needs to happen
    before the application starts. 
   */
  currentG = 0;
}

#ifdef FINAL
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
#endif

void update(unsigned char *pixels, int width, int height) {
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

#ifdef FINAL
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      unsigned char *pixel = getPixel(pixels, x, y, width);
      setPixel(pixel, 255, currentG, 0);
    }
  }

  currentG = (currentG + 2) % 255;
#else
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      unsigned char *currentPixel = pixels + (x + y * width) * 4;

      *currentPixel = 255;
    }
  }
#endif
}