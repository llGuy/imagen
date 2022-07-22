#include <base.hpp>
#include <iostream>

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

vec2 convertFromClipToPixel(vec2 ndc) {
  return vec2(width, height) * (ndc / 2.0f + vec2(0.5f));
}

void drawTriangle(unsigned char *pixels, vec2 *clipVerts, attribute *attribs) {
  // Convert to pixel coordinates
  vec2 pixelVerts[3] = {
    convertFromClipToPixel(clipVerts[0]),
    convertFromClipToPixel(clipVerts[1]),
    convertFromClipToPixel(clipVerts[2])
  };

  vec2 lower = min(pixelVerts[0], min(pixelVerts[1], pixelVerts[2]));
  vec2 upper = max(pixelVerts[0], max(pixelVerts[1], pixelVerts[2]));

  // Make sure to clamp these to the min/max of the window itself
  lower = clamp(lower, vec2(0), vec2(width, height));
  upper = clamp(upper, vec2(0), vec2(width, height));

  vec2 u = pixelVerts[1] - pixelVerts[0];
  vec2 v = pixelVerts[2] - pixelVerts[0];

  vec2 w1 = u;
  vec2 w2 = v - proj(v, u);

  for (int y = lower.y; y < upper.y; ++y) {
    for (int x = lower.x; x < upper.x; ++x) {

      vec2 point = vec2(x, y);
      vec2 delta = point - pixelVerts[0];

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
    vec2(-0.5f, -0.5f),
    vec2(0.0f, 0.5f),
    vec2(0.5f, -0.5f)
  };

  attribute attribs[] = {
    { vec3(255, 0, 0) },
    { vec3(0, 255, 0) },
    { vec3(0, 0, 255) }
  };

  drawTriangle(pixels, verts, attribs);
}
