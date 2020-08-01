#ifndef KIWI_DRIVER_SDL2_DRIVER_INTERNAL_H
#define KIWI_DRIVER_SDL2_DRIVER_INTERNAL_H

#include "kiwi/core/driver.h"

#ifdef __cplusplus
extern "C" {
#endif

struct KWSDL2_Driver {
  SDL_Window * window;
  SDL_Renderer * renderer;
};

typedef struct KWSDL2_Driver KWSDL2_Driver;

#define KWSDL2_PREAMBLE KWSDL2_PREAMBLE_FOR(driver)

#define KWSDL2_PREAMBLE_FOR(driver) \
  KWSDL2_Driver * sdl2 = driver->priv; \
  SDL_Renderer * renderer = sdl2->renderer; \
  SDL_Window * window = sdl2->window; \
  \


void KW_Rect2Rect(KW_Rect const * src, SDL_Rect * dst);

#ifdef __cplusplus
};
#endif

#endif
