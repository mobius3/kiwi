#ifndef KIWI_DRIVER_SDL2_FONT_H
#define KIWI_DRIVER_SDL2_FONT_H

#include "kiwi/core/driver.h"

#ifdef __cplusplus
extern "C" {
#endif

KW_Font * KWSDL2_CreateFont(KW_Driver * driver, void * data, float size, KW_Color color);
void KWSDL2_DestroyFont(KW_Driver * driver, KW_Font * font);
void KWSDL2_RenderText(KW_Driver * driver, KW_Font * font, uint8_t const * text, KW_TextTarget target, KW_Rect const * clip);

#ifdef __cplusplus
};
#endif

#endif
