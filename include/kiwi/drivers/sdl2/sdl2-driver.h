#ifndef KIWI_DRIVERS_SDL2_MEMORY_H
#define KIWI_DRIVERS_SDL2_MEMORY_H

#include "kiwi/core/driver.h"
#include "kiwi/drivers/sdl2/sdl2-driver-export.h"
#include "SDL.h"

#ifdef __cplusplus
extern "C" {
#endif

extern KIWI_DRIVERS_SDL2_EXPORT KW_Driver * KWSDL2_CreateDriver(SDL_Window * window);

#ifdef __cplusplus
};
#endif

#endif