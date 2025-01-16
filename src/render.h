#ifndef _RENDER
#define _RENDER
#include "config.h"
#include "vector.h"
#include "graphics.h"

void drawTextureLine(texture* texture, int textureView, int textureX, int x, int y0, int y1, int width);

void renderFloor();
void renderWalls();
void render();


#endif
