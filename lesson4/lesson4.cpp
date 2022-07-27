/*
** Here, we do the implementation of the 3D explanation from
** the previous lesson. Quite a lot of implementation.
** Need to add uniform data interface,
** Need to create transform functions (per triangle + vertex)
*/

#include <base.hpp>

float angle;

void setup() {
  angle = 0.0f;
}

struct uniform {
  // Projection data
  float nearPlane;

  // Transformation data
  float scale;
  mat3 rotation;
  vec3 translation;
};

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

vec3 transformVertex(vec3 vert, uniform *uniformData) {
  vert = uniformData->scale * (uniformData->rotation * vert) + uniformData->translation;

  vert.x *= uniformData->nearPlane / (vert.z * 0.57f);
  vert.y *= uniformData->nearPlane / (vert.z * 0.57f);

  return vert;
}

void transformAndDrawTriangle(
  unsigned char *pixels, vec3 *verts, attribute *attribs,
  uniform *uniformData) {
  vec2 transformed[3] = {
    vec2(transformVertex(verts[0], uniformData)),
    vec2(transformVertex(verts[1], uniformData)),
    vec2(transformVertex(verts[2], uniformData))
  };

  drawTriangle(pixels, transformed, attribs);
}

void update(unsigned char *pixels) {
  vec3 verts[] = {
    vec3(-0.5f, -0.5f, 0.0f),
    vec3(0.0f, 0.5f, 0.0f),
    vec3(0.5f, -0.5f, 0.0f)
  };

  attribute attribs[] = {
    { vec3(255, 0, 0) },
    { vec3(0, 255, 0) },
    { vec3(0, 0, 255) }
  };

  uniform uniformData = {
    1.0f,
    1.0f,
    rotate(radians(angle), vec3(0.0f, 1.0f, 0.0f)),
    vec3(0.0f, 0.0f, 2.0f)
  };

  transformAndDrawTriangle(pixels, verts, attribs, &uniformData);

  angle += 1.0f;
}
