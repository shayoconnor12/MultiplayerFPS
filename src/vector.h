#ifndef _VECTOR
#define _VECTOR
#include "config.h"

struct vec2
{
  float x;
  float y;
};
typedef struct vec2 vec2;

struct vec2i
{
  int x;
  int y;
};
typedef struct vec2i vec2i;

struct entity
{
  vec2 position;
  vec2 direction;
};
typedef struct entity entity;

extern vec2 cameraPlane;
extern entity player;

float getVec2Length(vec2* targetVector);
void normalizeVec2(vec2* targetVector);
void rotateVec2(vec2* targetVector, float theta);

#endif
