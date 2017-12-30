#ifndef KW_RENDERDRIVER_SDL2
#define KW_RENDERDRIVER_SDL2

#include "KW_macros.h"

#ifdef __cplusplus
extern "C" {
#endif

struct SDL_Renderer;
struct SDL_Window;

/**
 * \brief   Creates a SDL2 based KiWi render driver
 * \details First you need to create the SDL2 Renderer and the Window, and then pass them to this function
 * \param   renderer The SDL2 Renderer
 * \param   window The SDL2 window
 * \returns A KW_RenderDriver instance
 */
extern DECLSPEC struct KW_RenderDriver *KW_CreateSDL2RenderDriver(struct SDL_Renderer *renderer, struct SDL_Window *window);

/**
 * \brief   Gives back the SDL2 renderer used to create this RenderDriver
 * \param   driver The KW_RenderDriver instance to obtain the renderer from
 * \returns The SDL_Renderer used
 * \sa      KW_RenderDriverGetSDL2Window
 */
extern DECLSPEC struct SDL_Renderer *KW_RenderDriverGetSDL2Renderer(struct KW_RenderDriver *driver);

/**
 * \brief   Gives back the SDL2 window used to create this RenderDriver
 * \param   driver The KW_RenderDriver instance to obtain the window from
 * \returns The SDL_Window used
 * \sa      KW_RenderDriverGetSDL2Renderer
 */
extern DECLSPEC struct SDL_Window *KW_RenderDriverGetSDL2Window(struct KW_RenderDriver *driver);

#ifdef __cplusplus
}
#endif

#endif
