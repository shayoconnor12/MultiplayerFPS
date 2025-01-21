#include "graphics.h"

context video;

texture cobbleTexture;     
texture blueBrickTexture;  
texture floorTexture;      
texture ceilingTexture;    
texture woodFloorCarpetTL; 
texture woodFloorCarpetTR; 
texture woodFloorCarpetBL; 
texture woodFloorCarpetBR; 
texture woodFloor;
texture spriteTest;

texture* wallTextures[2] = {&cobbleTexture, &blueBrickTexture};
texture* floorTextures[6] = 
{
  &floorTexture, 
  &woodFloorCarpetTL,
  &woodFloorCarpetTR, 
  &woodFloorCarpetBL,
  &woodFloorCarpetBR,
  &woodFloor
};
texture* ceilingTextures[1] = {&ceilingTexture};
texture* spriteTextures[1] = {&spriteTest};

texture* textures[9] = {
             &cobbleTexture, &blueBrickTexture, 
             &floorTexture, &woodFloorCarpetTL, &woodFloorCarpetTR, &woodFloorCarpetBL, &woodFloorCarpetBR, &woodFloor,
             &ceilingTexture
           };

int loadBMP(texture* tex)
{
  uint32_t imageDataAddress;
  int width;
  int height;
  uint32_t pixelCount;
  uint16_t bitDepth;
  uint8_t byteDepth;
  uint32_t* pixels;
  
  printf("Loading BMP file: %s\n", tex->filename);
  
  FILE* file = fopen(tex->filename, "rb");
  if (!file)
  {
    printf("File : %s failed to open\n", tex->filename);
    return 0;
  }
  if(!(fgetc(file) == 'B' && fgetc(file) == 'M'))
  {
    printf("File : %s is not a valid bitmap\n", tex->filename);
    return 0;
  }
  fseek(file, 8, SEEK_CUR);
  fread(&imageDataAddress, 4, 1, file);
  fseek(file, 4, SEEK_CUR);
  fread(&width, 4, 1, file);
  fread(&height, 4, 1, file);
  fseek(file, 2, SEEK_CUR);
  fread(&bitDepth, 2, 1, file);
  
  if (bitDepth != 32)
  {
    printf("Error reading non-32 bit BMP file %s\n", tex->filename);
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

  printf("width: %d height: %d", tex->width, tex->height);
  printf("first pixel : 0x%08X\n", tex->pixels[0]);
  
  if (file == NULL)
  {
    printf("file null: %d\n");
  }
 
  fclose(file);
  printf("LALALALLLLLLLLLL\n");
  return 1;
}

void initTextures()
{
  cobbleTexture    .filename = "textures/cobbleTexture.bmp";
  blueBrickTexture .filename = "textures/blueBrickTexture.bmp";
  floorTexture     .filename = "textures/floorTexture.bmp";
  ceilingTexture   .filename = "textures/ceilingTexture.bmp";
  woodFloorCarpetTL.filename = "textures/woodFloorCarpetBL.bmp";
  woodFloorCarpetTR.filename = "textures/woodFloorCarpetBR.bmp";
  woodFloorCarpetBL.filename = "textures/woodFloorCarpetTL.bmp";
  woodFloorCarpetBR.filename = "textures/woodFloorCarpetTR.bmp";
  woodFloor        .filename = "textures/woodFloor.bmp";
  spriteTest.filename = "textures/spriteTest.bmp";
  
  for (int i = 0; i < 9; i++)
  {
    printf("texture name: %s\n", textures[i]->filename);
    if (!loadBMP(textures[i]))
    {
      printf("failed to load texture %s\n", textures[i]->filename);
    }
    else
    {
      printf("success\n");
    }
  }
  
  loadBMP(&spriteTest);

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
