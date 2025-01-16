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
  int width;
  int height;
  uint32_t* pixels;
};
typedef struct texture texture;

extern context video;
extern texture cobbleTexture;
extern texture blueBrickTexture;
extern texture floorTexture;  

int loadBMP(texture* tex, const char* filename);
void initTextures();
void init(const char* title, int x, int y);
void destroy();



#endif
