#ifndef KW_RENDERDRIVER_SDL2
#define KW_RENDERDRIVER_SDL2

struct SDL_Renderer;
struct SDL_Window;

#include "KW_macros.h"

/**
 * \brief   Creates a SDL2 based KiWi render driver
 * \details First you need to create the SDL2 Renderer and the Window, and then pass them to this function
 * \param   renderer The SDL2 Renderer
 * \param   window The SDL2 window
 * \returns A KW_RenderDriver instance
 */
extern DECLSPEC struct KW_RenderDriver * KW_CreateSDL2RenderDriver(struct SDL_Renderer * renderer, struct SDL_Window * window);

#endif

