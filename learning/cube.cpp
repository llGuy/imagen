#include "im.h"

#define MIN(a,b) ((a<b)?(a):(b))
#define MAX(a,b) ((a>b)?(a):(b))

struct display {
  unsigned char *pixels;
  int width, height;
};

unsigned char *getPixel(display d, int x, int y) {
  return d.pixels + (x + y * d.width) * 4;
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

struct attrib {
  // World space position
  vec3 wsPos;

  // Color
  vec3 color;

  vec3 normal;
};

attrib interpolateAttributes(attrib *att, float alpha, float beta, float gamma) {
  attrib res = {
    att[0].wsPos * alpha + att[1].wsPos * beta + att[2].wsPos * gamma,
    att[0].color * alpha + att[1].color * beta + att[2].color * gamma,
    att[0].normal * alpha + att[1].normal * beta + att[2].normal * gamma,
  };

  return res;
}

vec3 kLightPosition = vec3(-3.0f, 3.0f, 0.0f);

vec3 calculateColor(attrib att) {
  vec3 lightDir = normalize(kLightPosition - att.wsPos);
  float diffuse = clamp(dot(lightDir, att.normal), 0.0f, 1.0f) * 10.0f;

  vec3 viewDir = normalize(-att.wsPos);
  vec3 reflectDir = normalize(reflect(-lightDir, att.normal));

  float specular = pow(max(dot(viewDir, reflectDir), 0.0f), 32) * 3.0f;
  specular = clamp(specular, 0.0f, 1.0f);
  float add = 0.02f + diffuse + specular;
 
  vec3 res = add * att.color;

  return res;
}

void drawTriangle(display win, vec2 *ssPos, attrib *att) {
  vec2 min;
  min.x = MIN((MIN(ssPos[0].x, ssPos[1].x)), ssPos[2].x);
  min.y = MIN((MIN(ssPos[0].y, ssPos[1].y)), ssPos[2].y);

  vec2 max;
  max.x = MAX((MAX(ssPos[0].x, ssPos[1].x)), ssPos[2].x);
  max.y = MAX((MAX(ssPos[0].y, ssPos[1].y)), ssPos[2].y);

  vec2 u = ssPos[1]-ssPos[0];
  vec2 v = ssPos[2]-ssPos[0];

  vec2 w1 = u;
  vec2 w2 = v - proj(v, u);

  // Helper
  float w1dotw1Inv = 1.0f/dot(w1,w1);
  float w2dotw2Inv = 1.0f/dot(w2,w2);
  float udotv = dot(u,v);
  float udotvInv = 1.0f/dot(u,v);
  float udotuInv = 1.0f/dot(u,u);

  for (int y = min.y; y < max.y; ++y) {
    for (int x = min.x; x < max.x; ++x) {
      if (x >= 0 && x < win.width && y >= 0 && y <= win.height) {
        vec2 point = {x, y};
        vec2 delta = point - ssPos[0];

        float beta = dot(delta,w1)*w1dotw1Inv -
          (dot(delta,w2)*w2dotw2Inv)*(udotv*udotuInv);
        float gamma = dot(delta,w2)*w2dotw2Inv;

        if (beta + gamma <= 1.0f && beta >= 0.0f && gamma >= 0.0f) {
          // Do interpolation
          float alpha = 1.0f - beta - gamma;

          attrib interpolated = interpolateAttributes(att, alpha, beta, gamma);
          interpolated.color = calculateColor(interpolated);
          interpolated.color *= 255.0f;

          unsigned char *pixel = getPixel(win, x, y);
          setPixel(pixel, interpolated.color.r, interpolated.color.g, interpolated.color.b);
        }
      }
    }
  }
}

vec3 project3DPoint(vec3 in, float near) {
  vec3 res = {in.x * near / in.z, in.y * near / in.z, in.z};
  return res;
}

void submitVertices(int vertexCount, attrib *vertices, display win) {
  for (int i = 0; i < vertexCount; i += 3) {
    vec2 ssPos[3];

    attrib *current = &vertices[i];

    vec3 normal = -normalize(cross(
      current[1].wsPos-current[0].wsPos,
      current[2].wsPos-current[0].wsPos));

    for (int v = 0; v < 3; ++v) {
      vec3 projected = project3DPoint(current[v].wsPos, 1.0f);
      ssPos[v] = (0.5f*vec2(projected) + vec2(0.5f)) * vec2(win.width, win.height);
      current[v].normal = normal;
    }

    drawTriangle(win, ssPos, current);
  }
}



float dist = 0.0f;

void setup() {
  
}

void update(unsigned char *pixels, int width, int height) {
  display win = {pixels, width, height};

  attrib vertices[3] = {
    {{3.0f*-1.0f, 0.0f, 5.0f}, {0.2f, 0.2f, 0.2f}},
    {{3.0f*0.0f, 3.0f*-1.0f, 3.0f}, {0.2f, 0.2f, 0.2f}},
    {{3.0f*+1.0f, 0.0f, 5.0f}, {0.2f, 0.2f, 0.2f}},
  };

  submitVertices(3, vertices, win);

  dist += 0.01f;

  kLightPosition.x += 0.1f;
}
