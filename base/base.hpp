#ifndef _IM_H_
#define _IM_H_

#include <string.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/projection.hpp>

#include <GLFW/glfw3.h>

using namespace glm;

// Makes it so that typing "u8" is just like typing "unsigned char"
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned int   bool32_t;

int isKeyPressedImpl(int key);
#define isKeyPressed(key) isKeyPressedImpl(GLFW_KEY_##key)

#define MIN(a, b) ((a < b) ? (a) : (b))
#define MAX(a, b) ((a > b) ? (a) : (b))

#endif
