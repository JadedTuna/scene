#include <Python.h>
#include <SDL/SDL.h>
#include "SDL/SDL_gfxPrimitives.h"

int SX = 640;
int SY = 480;

typedef struct {
    unsigned int r;
    unsigned int g;
    unsigned int b;
    unsigned int a;
} RGBA_Color;

SDL_Surface* screen;
SDL_Rect     bounds;
SDL_Event    event;

RGBA_Color    fill_color   = {255, 255, 255, 255};
RGBA_Color    stroke_color = {255, 255, 255, 255};
char          fill         = 1;
char          stroke       = 0;
