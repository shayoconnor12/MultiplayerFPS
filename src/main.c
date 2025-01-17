#include "config.h"
#include "render.h"
#include "vector.h"
#include "graphics.h"

int main(int argc, char** argv)
{
  init("Hello, World", 200, 200); 
  player.position = (vec2) { 2, 2 };
  player.direction = (vec2) {1.0f, 0.1f};
  normalizeVec2(&player.direction);  
  cameraPlane = player.direction;
  rotateVec2(&cameraPlane, PI/2); 
  
  int running = 1;
  uint32_t lastTime = SDL_GetTicks();
  int frameCounter = 0;

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
      rotateVec2(&player.direction, 0.035f);
      rotateVec2(&cameraPlane, 0.035f);
    }
    if (keystate[SDL_SCANCODE_A])
    {
      rotateVec2(&player.direction, -0.035f);
      rotateVec2(&cameraPlane, -0.035f);
    }
    if (keystate[SDL_SCANCODE_W])
    {
      vec2 newPosition; 
      newPosition.x = player.position.x + player.direction.x * 0.04f;
      newPosition.y = player.position.y + player.direction.y * 0.04f;
      
      if (worldMap[(int)(floor(player.position.y) * MAPWIDTH + floor(newPosition.x))] <= 0)
      {
        player.position.x = newPosition.x;
      }
      if (worldMap[(int)(floor(newPosition.y) * MAPWIDTH + floor(player.position.x))] <= 0)
      {
        player.position.y = newPosition.y;
      }
    }
    if (keystate[SDL_SCANCODE_S])
    {
      vec2 newPosition; 
      newPosition.x = player.position.x - player.direction.x * 0.04f;
      newPosition.y = player.position.y - player.direction.y * 0.04f;

      if (worldMap[(int)(floor(player.position.y) * MAPWIDTH + floor(newPosition.x))] <= 0)
      {
        player.position.x = newPosition.x;
      }
      if (worldMap[(int)(floor(newPosition.y) * MAPWIDTH + floor(player.position.x))] <= 0)
      {
        player.position.y = newPosition.y;
      }
    }
    frameCounter++;
    uint32_t currentTime = SDL_GetTicks();
    if (frameCounter == 10)
    {
      printf("%d\n", (currentTime - lastTime));
      frameCounter = 0;
    }
    lastTime = currentTime;
  } 
  destroy();  
}

