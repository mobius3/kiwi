#ifndef KIWI_DRIVER_SDL2_TEXTURE_H
#define KIWI_DRIVER_SDL2_TEXTURE_H

#include "kiwi/core/driver.h"

#ifdef __cplusplus
extern "C" {
#endif

KW_Texture * KWSDL2_CreateTexture(KW_Driver * driver, const uint8_t * pixels, size_t width, size_t height, uint8_t bytesPerPixel);
void KWSDL2_DestroyTexture(KW_Driver * driver, KW_Texture * texture);
void KWSDL2_RenderTexture(KW_Driver * driver, KW_Texture * texutre, KW_Rect const * src, KW_Rect const * dst, KW_Rect const * clip);

#ifdef __cplusplus
};
#endif

#endif
