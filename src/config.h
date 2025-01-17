#ifndef _CONFIG
#define _CONFIG
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <SDL2/SDL.h>

#define SIGN(_x) (_x > 0) - (_x < 0)
#define ASSERT(_x, ...) if(!(_x)) { fprintf(stderr, __VA_ARGS__); exit(0); }
#define MIN(a, b) ({ __typeof__(a) _a = (a), _b = (b); _a < _b ? _a : _b; })
#define MAX(a, b) ({ __typeof__(a) _a = (a), _b = (b); _a > _b ? _a : _b; })
#define MAPWIDTH 20
#define MAPHEIGHT 12

#define PI 3.1415926589

#define WIDTH 1260
#define HEIGHT 720

static int worldMap[MAPWIDTH*MAPHEIGHT] = 
{
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,-5,-5,-5,-5,-5,-5,-5, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-5,-5,-1,-2,-5, 2,-1,-2, 1,
 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2,-5,-4,-3,-5, 2,-4,-3, 1,
 1, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 2,-5,-5,-5,-5, 2,-5,-5, 1,
 1, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
 2, 2, 2, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

#endif
