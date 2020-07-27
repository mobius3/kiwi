#ifndef KIWI_DRIVER_SDL2_MEMORY_H
#define KIWI_DRIVER_SDL2_MEMORY_H

#include "kiwi/core/driver.h"
#include "kiwi/drivers/sdl2/sdl2-driver-export.h"

#ifdef __cplusplus
extern "C" {
#endif

extern KIWI_DRIVERS_SDL2_EXPORT void * KWSDL2_Allocate(KW_Driver * driver, size_t size);
extern KIWI_DRIVERS_SDL2_EXPORT void KWSDL2_Free(KW_Driver * driver, void * ptr);

#ifdef __cplusplus
};
#endif

#endif
