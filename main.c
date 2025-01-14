#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <SDL2/SDL.h>

#define SIGN(_x) (_x > 0) - ( _x < 0)
#define ASSERT(_x, ...) if(!(_x)) { fprintf(stderr, __VA_ARGS__); exit(0); }
#define MIN(a, b) ({ __typeof__(a) _a = (a), _b = (b); _a < _b ? _a : _b; })
#define MAX(a, b) ({ __typeof__(a) _a = (a), _b = (b); _a > _b ? _a : _b; })
#define MAPWIDTH 8
#define MAPHEIGHT 8

#define PI 3.1415926589

#define WIDTH 800
#define HEIGHT 600

static uint8_t worldMap[MAPWIDTH*MAPHEIGHT] = 
{
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 1, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 2, 0, 2, 2, 0, 1,
  1, 0, 0, 0, 1, 0, 0, 1,
  1, 0, 1, 0, 0, 0, 0, 1,
  1, 1, 1, 1, 1, 1, 1, 1
};

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

struct
{
  vec2 position;
  vec2 direction;
} player;

vec2 cameraPlane;

struct
{
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* texture;
  uint32_t pixels[WIDTH*HEIGHT];
} video;

struct texture 
{
  int width;
  int height;
  uint32_t* pixels;
};
typedef struct texture texture;

texture wallTexture;

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

void init(const char* title, int x, int y)
{ 
  if(!loadBMP(&wallTexture, "wallTexture.bmp"))
  {
    fprintf(stderr, "failed to load wallTexture");
  }

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

void drawVerticalLine(int x, int y0, int y1, uint32_t color, int width)
{
  for (int i = 0; i < width; i++)
  {
    for (int y = y0; y <= y1; y++)
    {
      video.pixels[(y * WIDTH) + x + i] = color;
    }
  }
}

void drawTextureLine(texture* texture, int textureView, int textureX, int x, int y0, int y1, int width)
{
  int lineHeight = y1-y0;
  float step = (float) textureView / lineHeight;
  for (int i = 0; i < width; i++)
  {
    float textureY0 = (float) (texture->height - textureView) / 2;
    int textureY = (int)textureY0;
    for (int y = y0; y <= y1; y++)
    {
      textureY0 += step;
      textureY = MIN((int) textureY0, texture->height - 1);
      video.pixels[y * WIDTH + x + i] = texture->pixels[textureY * texture->width + textureX];
    }
  }
}

void render()
{
  for (int x = 0; x < WIDTH; x+=3)
  {
    const float cameraX = (2*(x/(float)(WIDTH))) - 1;
    const vec2 rayDirection = 
    {
      player.direction.x + cameraPlane.x * cameraX,
      player.direction.y + cameraPlane.y * cameraX
    };
    
    vec2 pos = player.position;
    vec2i ipos = { (int) pos.x, (int) pos.y };

    const vec2 deltaDistance = 
    {
      fabsf(rayDirection.x) == 0 ? 1e30 : fabsf(1.0f / rayDirection.x),
      fabsf(rayDirection.y) == 0 ? 1e30 : fabsf(1.0f / rayDirection.y)
    };
    vec2 sideDistance = 
    {
      deltaDistance.x * (rayDirection.x < 0 ? (pos.x - ipos.x) : (ipos.x + 1 - pos.x)),
      deltaDistance.y * (rayDirection.y < 0 ? (pos.y - ipos.y) : (ipos.y + 1 - pos.y))
    };
    
    const vec2i step = { (int) SIGN(rayDirection.x), (int) SIGN(rayDirection.y) };
    
    struct { int value, side; vec2 position; } hit = { 0, 0, { 0.0f, 0.0f } };
     
    while (!hit.value)
    {
      if (sideDistance.x < sideDistance.y)
      {
        sideDistance.x += deltaDistance.x;
        ipos.x += step.x;
        hit.side = 0;
      }
      else
      {
        sideDistance.y += deltaDistance.y;
        ipos.y += step.y;
        hit.side = 1;
      }
      ASSERT(
        ipos.x >= 0 && ipos.x < 8 && ipos.y >= 0 && ipos.y < 8,
        "DDA out of bounds at pos: x  %d y %d, screen x %d\n", ipos.x, ipos.y, x);
      hit.value = worldMap[ipos.y * MAPWIDTH + ipos.x];
    }

    const float perpetualDistance = 
      hit.side == 0 ? (sideDistance.x - deltaDistance.x) : (sideDistance.y - deltaDistance.y);
    float wallX;
    if (hit.side == 0)
    {
      wallX = pos.y + perpetualDistance * rayDirection.y;
    }
    else
    {
      wallX = pos.x + perpetualDistance * rayDirection.x;
    }
    wallX -= floor(wallX);

    int textureX = (int) (wallX * 64);

    const int height = (int) (HEIGHT/perpetualDistance) * 0.7;
    int y0 = MAX((HEIGHT/2) - (height / 2), 0);
    int y1 = MIN((HEIGHT/2) + (height / 2), HEIGHT - 1);  
   
    double heightRatio = (double) HEIGHT/height;
    int textureView = heightRatio < 1 ? heightRatio * wallTexture.height : wallTexture.height;
    
    drawVerticalLine(x, 0, y0, 0x006495ED, 3);
    drawTextureLine(&wallTexture, textureView, textureX, x, y0, y1, 3);
    drawVerticalLine(x, y1, HEIGHT - 1, 0x004F200F, 3);
  }  
}


void rotateVec2(vec2* v, float theta)
{
    float x = v->x;
    float y = v->y;
    
    v->x = x * cos(theta) - y * sin(theta);
    v->y = x * sin(theta) + y * cos(theta);
}

int main(int argc, char** argv)
{
  init("Hello, World", 200, 200); 
  player.position = (vec2) { 2, 2 };
  player.direction = (vec2) {1.0f, 0.1f};
  normalizeVec2(&player.direction);  
  cameraPlane = player.direction;
  rotateVec2(&cameraPlane, PI/2); 
  
  int running = 1;
  while (running)
  {
    memset(video.pixels, 0, sizeof(video.pixels));
    render();
    SDL_UpdateTexture(video.texture, NULL, video.pixels, WIDTH*4);
    SDL_RenderCopy(video.renderer, video.texture, NULL, NULL);
    SDL_RenderPresent(video.renderer);
  
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
          running = 0;
          break;
      }
    }
    const uint8_t * keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_D])
    {
      rotateVec2(&player.direction, 0.045f);
      rotateVec2(&cameraPlane, 0.045f);
    }
    if (keystate[SDL_SCANCODE_A])
    {
      rotateVec2(&player.direction, -0.045f);
      rotateVec2(&cameraPlane, -0.045f);
    }
    if (keystate[SDL_SCANCODE_W])
    {
      player.position.x += player.direction.x * 0.04f;
      player.position.y += player.direction.y * 0.04f;
    }
    if (keystate[SDL_SCANCODE_S])
    {
      player.position.x -= player.direction.x * 0.04f;
      player.position.y -= player.direction.y * 0.04f;
    }
  } 
  destroy();  
}

