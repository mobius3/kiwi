#ifndef KW_RENDERDRIVER_SDL2
#define KW_RENDERDRIVER_SDL2

#include "SDL.h"

struct KW_RenderDriver * KW_CreateSDL2RenderDriver(SDL_Renderer * renderer, SDL_Window * window);

#endif

