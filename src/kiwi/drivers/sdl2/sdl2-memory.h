#ifndef KIWI_DRIVER_SDL2_MEMORY_H
#define KIWI_DRIVER_SDL2_MEMORY_H

#include "kiwi/core/driver.h"

#ifdef __cplusplus
extern "C" {
#endif

void * KWSDL2_Allocate(KW_Driver * driver, size_t size);
void KWSDL2_Free(KW_Driver * driver, void * ptr);

#ifdef __cplusplus
};
#endif

#endif
