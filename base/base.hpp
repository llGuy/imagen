#ifndef _IM_H_
#define _IM_H_

#include <string.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/projection.hpp>

#include <GLFW/glfw3.h>

extern int width;
extern int height;

using namespace glm;

// Makes it so that typing "u8" is just like typing "unsigned char"
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned int   bool32_t;

vec2 mouseMovement();

int isKeyPressedImpl(int key);
#define isKeyPressed(key) isKeyPressedImpl(GLFW_KEY_##key)

#if !defined(MIN)
#define MIN(a, b) ((a < b) ? (a) : (b))
#endif

#if !defined(MAX)
#define MAX(a, b) ((a > b) ? (a) : (b))
#endif

void toggleFPSMode();

#endif
