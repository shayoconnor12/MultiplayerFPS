#include "graphics.h"

context video;

texture cobbleTexture;
texture blueBrickTexture;
texture floorTexture;

int loadBMP(texture* tex, const char* filename)
{
  uint32_t imageDataAddress;
  int width;
  int height;
  uint32_t pixelCount;
  uint16_t bitDepth;
  uint8_t byteDepth;
  uint32_t* pixels;

  printf("Loading BMP file: %s\n", filename);
  
  FILE* file = fopen(filename, "rb");
  if (!file)
  {
    printf("File : %s failed to open\n", filename);
    return 0;
  }
  if(!(fgetc(file) == 'B' && fgetc(file) == 'M'))
  {
    printf("File : %s is not a valid bitmap\n", filename);
    return 0;
  }
  printf("test\n");
  fseek(file, 8, SEEK_CUR);
  fread(&imageDataAddress, 4, 1, file);
  fseek(file, 4, SEEK_CUR);
  fread(&width, 4, 1, file);
  fread(&height, 4, 1, file);
  fseek(file, 2, SEEK_CUR);
  fread(&bitDepth, 2, 1, file);
  
  if (bitDepth != 32)
  {
    printf("Error reading non-32 bit BMP file %s\n", filename);
    printf("Bit depth: %d\n", bitDepth);
    return 0;
  }
  pixelCount = width*height;
  byteDepth = bitDepth / 8;
  pixels = (uint32_t*)malloc(pixelCount * byteDepth);
  if (!pixels)
  {
    printf("Failed to allocate %d pixels\n", pixelCount);
    return 0;
  }
  fseek(file, imageDataAddress, SEEK_SET);
  int pixelsRead = fread(pixels, byteDepth, pixelCount, file);
  if (!(pixelsRead == pixelCount))
  {
    printf("Error, pixelsRead %d does not equal pixelCount %d\n", pixelsRead, pixelCount);
    free(pixels);
    return 0;
  }
  tex->width = width;
  tex->height = height;
  tex->pixels = pixels;


  printf("pixel 1 = 0x%08X\n", tex->pixels[0]);
  fclose(file);
  return 1;
}

void initTextures()
{
  if(!loadBMP(&cobbleTexture, "textures/cobbleTexture.bmp"))
  {
    fprintf(stderr, "failed to load cobbleTexture");
  }
  if(!loadBMP(&blueBrickTexture, "textures/blueBrickTexture.bmp"))
  {
    fprintf(stderr, "failed to load blueBrickTexture");
  }
  if(!loadBMP(&floorTexture, "textures/floorTexture.bmp"))
  {
    fprintf(stderr, "failed to load floorTexture");
  }

}

void init(const char* title, int x, int y)
{ 
  initTextures();

  ASSERT(!SDL_Init(SDL_INIT_VIDEO), "SDL Initialization failed: %s\n", SDL_GetError());

  video.window = SDL_CreateWindow(title, x, y, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  ASSERT(video.window, "Window creation failed: %s\n", SDL_GetError());
  
  video.renderer = SDL_CreateRenderer(video.window, -1, SDL_RENDERER_PRESENTVSYNC);
  ASSERT(video.renderer, "Renderer creation failed: %s\n", SDL_GetError());  
  
  video.texture = SDL_CreateTexture(video.renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);  
  ASSERT(video.texture, "Texture creation failed: %s\n", SDL_GetError());
}

void destroy()
{
  SDL_DestroyWindow(video.window);
  SDL_DestroyRenderer(video.renderer);
  SDL_DestroyTexture(video.texture);
  SDL_Quit();
}
