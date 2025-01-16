#include "vector.h"

float getVec2Length(vec2* targetVector)
{
  float w = sqrt( targetVector->x * targetVector->x + targetVector->y * targetVector-> y);
  return w;
}

void normalizeVec2(vec2* targetVector)
{
  float w = getVec2Length(targetVector);
  targetVector->x /= w;
  targetVector->y /= w;
}

vec2 cameraPlane;
entity player;

void rotateVec2(vec2* targetVector, float theta)
{
    float x = targetVector->x;
    float y = targetVector->y;

    targetVector->x = x * cos(theta) - y * sin(theta);
    targetVector->y = x * sin(theta) + y * cos(theta);
}

