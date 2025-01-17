#include "render.h"

void drawTextureLine(texture* texture, int textureView, int textureX, int x, int y0, int y1, int width)
{
}
void renderFloor()
{
  const int lineWidth = 2;
  for (int y = 0; y < HEIGHT; y+=lineWidth)
  {
    vec2 rayDirection0 = 
    {
      player.direction.x - cameraPlane.x,
      player.direction.y - cameraPlane.y
    };
    vec2 rayDirection1 = 
    {
      player.direction.x + cameraPlane.x,
      player.direction.y + cameraPlane.y
    };
    
    int position = y - HEIGHT / 2;
    float positionZ = 0.5 * HEIGHT;

    float rowDistance = positionZ / position;

    vec2 floorStep = 
    {
      rowDistance * (rayDirection1.x - rayDirection0.x) / WIDTH,
      rowDistance * (rayDirection1.y - rayDirection0.y) / WIDTH
    };

    vec2 floorPosition = 
    {
      player.position.x + (rowDistance * rayDirection0.x),
      player.position.y + (rowDistance * rayDirection0.y)
    };

    texture* floorPointer = &floorTexture;

    for (int x = 0; x < WIDTH; x++)
    {
      vec2i cell = { floor(floorPosition.x), floor(floorPosition.y) };
      if (cell.x >= 0 && cell.x <= MAPWIDTH && cell.y >= 0 && cell.y <= MAPHEIGHT)
      {
        switch(worldMap[cell.y * MAPWIDTH + cell.x])
        {
          case -1: floorPointer = &woodFloorCarpetTL; break;
          case -2: floorPointer = &woodFloorCarpetTR; break;
          case -3: floorPointer = &woodFloorCarpetBR; break;
          case -4: floorPointer = &woodFloorCarpetBL; break;
          case -5: floorPointer = &woodFloor;         break;
          default: floorPointer = &floorTexture;      break;
        }
      }
      vec2i textureCoordinate = 
      { 
        (int) (floorPointer->width  * (floorPosition.x - cell.x)) & 63, 
        (int) (floorPointer->height * (floorPosition.y - cell.y)) & 63
      };

      floorPosition.x += floorStep.x;
      floorPosition.y += floorStep.y;
      uint32_t color;

      for (int i = 0; i < lineWidth && y + i < HEIGHT; i++)
      {
        color = floorPointer->pixels[(floorPointer->width * textureCoordinate.y) + textureCoordinate.x];
        video.pixels[(y+i) * WIDTH + x] = color;

        color = ceilingTexture.pixels[(ceilingTexture.width * textureCoordinate.y) + textureCoordinate.x];
        video.pixels[(HEIGHT - (y+i) - 1) * WIDTH + x] = color;
      }
    }
  }
}

void renderWalls()
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

    while (hit.value <= 0)
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

    texture* texturePointer;
    switch (hit.value)
    {
      case 1: texturePointer = &cobbleTexture; break;
      case 2: texturePointer = &blueBrickTexture; break;
    }
    
    int textureX = (int) (wallX * texturePointer->width);

    const int height = (int) (HEIGHT/perpetualDistance);
    int y0 = MAX((HEIGHT/2) - (height / 2), 0);
    int y1 = MIN((HEIGHT/2) + (height / 2), HEIGHT - 1);

    double heightRatio = (double) HEIGHT/height;
    int textureView = heightRatio < 1 ? heightRatio * texturePointer->height : texturePointer->height;

    int lineHeight = y1-y0;
    float textureStep = (float) textureView / lineHeight;
    for (int i = 0; i < 3; i++)
    {
      float textureY0 = (float) (texturePointer->height - textureView) / 2;
      int textureY = (int)textureY0;
      for (int y = y0; y <= y1; y++)
      {
        textureY0 += textureStep;
        textureY = MIN((int) textureY0, texturePointer->height - 1);
        uint32_t color = texturePointer->pixels[textureY * texturePointer->width + textureX];

        if (hit.side == 1) color = (color >> 1) & 0x007F7F7F;
        video.pixels[y * WIDTH + x + i] = color;
      }
    }
  }
}

void render()
{
  renderFloor();
  renderWalls();
}

