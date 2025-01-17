#ifndef _GRAPHICS
#define _GRAPHICS
#include "config.h"

struct context
{
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* texture;
  uint32_t pixels[WIDTH*HEIGHT];
};
typedef struct context context;

struct texture 
{
  const char* filename;
  int width;
  int height;
  uint32_t* pixels;
};
typedef struct texture texture;

extern context video;
extern texture cobbleTexture;
extern texture blueBrickTexture;
extern texture floorTexture;  
extern texture ceilingTexture;
extern texture woodFloorCarpetTL;
extern texture woodFloorCarpetTR;
extern texture woodFloorCarpetBL;
extern texture woodFloorCarpetBR;
extern texture woodFloor;

extern texture* wallTextures[2];
extern texture* floorTextures[6];
extern texture* ceilingTextures[1];

int loadBMP(texture* tex);
void initTextures();
void init(const char* title, int x, int y);
void destroy();



#endif
