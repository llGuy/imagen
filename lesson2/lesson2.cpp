/*
** Here, we add attributes to each pixel and
** add interpolation between the attributes (rainbow triangle)
*/

#include <base.hpp>

void setup() {

}

struct attribute {
  vec3 color;
};

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

void drawTriangle(unsigned char *pixels, vec2 *verts, attribute *attribs) {
  vec2 lower = min(verts[0], min(verts[1], verts[2]));
  vec2 upper = max(verts[0], max(verts[1], verts[2]));

  vec2 u = verts[1] - verts[0];
  vec2 v = verts[2] - verts[0];

  vec2 w1 = u;
  vec2 w2 = v - proj(v, u);

  for (int y = lower.y; y < upper.y; ++y) {
    for (int x = lower.x; x < upper.x; ++x) {

      vec2 point = vec2(x, y);
      vec2 delta = point - verts[0];

      float gamma = dot(delta, w2) / dot(w2, w2);
      float beta = ( dot(delta, w1) / dot(w1, w1) ) - ( dot(delta, w2) / dot(w2, w2) ) * ( dot(u, v) / dot(u, u) );

      if (beta + gamma <= 1.0f && beta >= 0.0f && gamma >= 0.0f) {
        float alpha = 1.0f - beta - gamma;

        attribute interpolated;
        interpolated.color = alpha * attribs[0].color + beta * attribs[1].color + gamma * attribs[2].color;

        unsigned char *pixel = getPixel(pixels, x, y);
        setPixel(pixel, interpolated.color.r, interpolated.color.g, interpolated.color.b);
      }

    }
  }
}

void update(unsigned char *pixels) {
  vec2 verts[] = {
    vec2(32, 32),
    vec2(width/2, height-32),
    vec2(width-32, 32)
  };

  attribute attribs[] = {
    { vec3(255, 0, 0) },
    { vec3(0, 255, 0) },
    { vec3(0, 0, 255) }
  };

  drawTriangle(pixels, verts, attribs);
}
