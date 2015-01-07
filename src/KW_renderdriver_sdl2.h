#ifndef KW_RENDERDRIVER_SDL2
#define KW_RENDERDRIVER_SDL2

#ifdef __cplusplus
extern "C" {
#endif

struct SDL_Renderer;
struct SDL_Window;

#include "KW_macros.h"

extern DECLSPEC struct KW_RenderDriver * KW_CreateSDL2RenderDriver(struct SDL_Renderer * renderer, struct SDL_Window * window);

#ifdef __cplusplus
}
#endif

#endif

