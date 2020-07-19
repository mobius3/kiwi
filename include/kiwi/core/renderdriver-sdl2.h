#ifndef KIWI_CORE_RENDERDRIVER_SDL2
#define KIWI_CORE_RENDERDRIVER_SDL2

#include "kiwi/core/core-export.h"

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
extern KIWI_CORE_EXPORT struct KW_RenderDriver *KW_CreateSDL2RenderDriver(struct SDL_Renderer *renderer, struct SDL_Window *window);

/**
 * \brief   Gives back the SDL2 renderer used to create this RenderDriver
 * \param   driver The KW_RenderDriver instance to obtain the renderer from
 * \returns The SDL_Renderer used
 * \sa      KW_RenderDriverGetSDL2Window
 */
extern KIWI_CORE_EXPORT struct SDL_Renderer *KW_RenderDriverGetSDL2Renderer(struct KW_RenderDriver *driver);

/**
 * \brief   Gives back the SDL2 window used to create this RenderDriver
 * \param   driver The KW_RenderDriver instance to obtain the window from
 * \returns The SDL_Window used
 * \sa      KW_RenderDriverGetSDL2Renderer
 */
extern KIWI_CORE_EXPORT struct SDL_Window *KW_RenderDriverGetSDL2Window(struct KW_RenderDriver *driver);

#ifdef __cplusplus
}
#endif

#endif
