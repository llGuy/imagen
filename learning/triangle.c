#define MIN(a,b) ((a<b)?(a):(b))
#define MAX(a,b) ((a>b)?(a):(b))

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

struct v2 {
  float x, y;
};

float dot(struct v2 a, struct v2 b) {
  return (a.x * b.x) + (a.y * b.y);
}

void drawTriangle(
  unsigned char *pixels, int width,
  struct v2 a, struct v2 b, struct v2 c) {
  struct v2 min;
  min.x = MIN((MIN(a.x, b.x)), c.x);
  min.y = MIN((MIN(a.y, b.y)), c.y);

  struct v2 max;
  max.x = MAX((MAX(a.x, b.x)), c.x);
  max.y = MAX((MAX(a.y, b.y)), c.y);

  struct v2 u = {b.x-a.x, b.y-a.y};
  struct v2 v = {c.x-a.x, c.y-a.y};

  struct v2 w1 = u;
  float projCoeff = dot(u,v)/dot(u,u);
  struct v2 w2 = {v.x-projCoeff*u.x, v.y-projCoeff*u.y};

  // Helper
  float w1dotw1Inv = 1.0f/dot(w1,w1);
  float w2dotw2Inv = 1.0f/dot(w2,w2);
  float udotv = dot(u,v);
  float udotvInv = 1.0f/dot(u,v);
  float udotuInv = 1.0f/dot(u,u);

  for (int y = min.y; y < max.y; ++y) {
    for (int x = min.x; x < max.x; ++x) {
      struct v2 point = {x, y};

      struct v2 delta = {
        point.x - a.x,
        point.y - a.y
      };

      float alpha = dot(delta,w1)*w1dotw1Inv -
        (dot(delta,w2)*w2dotw2Inv)*(udotv*udotuInv);
      float beta = dot(delta,w2)*w2dotw2Inv;

      if (alpha + beta <= 1.0f && alpha >= 0.0f && beta >= 0.0f) {
        unsigned char *pixel = getPixel(pixels, x, y, width);
        setPixel(pixel, 255, 0, 0);
      }
    }
  }
}

void setup() {
  
}

void update(unsigned char *pixels, int width, int height) {
  /*
  struct v2 a = {50, 50};
  struct v2 b = {250, 50};
  struct v2 c = {150, 250};
  drawTriangle(pixels, width, a, b, c);
  */

  struct v2 a = {50, 50};
  struct v2 b = {250, 50};
  struct v2 c = {50, 250};
  struct v2 d = {250, 250};
  drawTriangle(pixels, width, a, b, c);
  drawTriangle(pixels, width, c, b, d);
}
