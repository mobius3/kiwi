#ifndef KIWI_DRIVER_H
#define KIWI_DRIVER_H

#include <stdlib.h>
#include <stdint.h>
#include "kiwi/core/color.h"
#include "kiwi/core/rect.h"
#include "kiwi/core/bool.h"
#include "kiwi/core/gui.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct KW_Driver KW_Driver;

struct KW_Texture {
  void * priv;
};

typedef struct KW_Texture KW_Texture;

struct KW_Font {
  void * priv;
};

typedef struct KW_Font KW_Font;

// TODO: move to core
enum KW_TextAlignment {
  KW_ALIGN_LEFT,
  KW_ALIGN_CENTER,
  KW_ALIGN_RIGHT
};

typedef enum KW_TextAlignment KW_TextAlignment;

struct KW_TextTarget {
  uint8_t baseline;
  uint8_t line_spacing;
  uint16_t width;
  KW_bool wrap;
  KW_TextAlignment alignment;
};

typedef struct KW_TextTarget KW_TextTarget;


typedef void * (*KW_DriverAllocateFunction)(KW_Driver * driver, size_t size);
typedef void (*KW_DriverFreeFunction)(KW_Driver * driver, void * ptr);
typedef KW_Texture * (*KW_DriverCreateTextureFunction)(KW_Driver * driver, const uint8_t * pixels, size_t width, size_t height, uint8_t bytesPerPixel);
typedef void (*KW_DriverDestroyTextureFunction)(KW_Driver * driver, KW_Texture * texture);
typedef void (*KW_DriverRenderTextureFunction)(KW_Driver * driver, KW_Texture * texture, KW_Rect const * src, KW_Rect const * dst, KW_Rect const * clip);
typedef KW_Font * (*KW_DriverCreateFontFunction)(KW_Driver * driver, void * data, float size, KW_Color color);
typedef void (*KW_DriverDestroyFontFunction)(KW_Driver * driver, KW_Font * font);
typedef void (*KW_DriverRenderTextFunction)(KW_Driver * driver, KW_Font * font, uint8_t const * text, KW_TextTarget target, KW_Rect const * clip);
typedef void (*KW_DriverUpdateFunction)(KW_Driver * driver, KW_GUI * gui);

struct KW_Driver {
  KW_DriverAllocateFunction allocate;
  KW_DriverFreeFunction free;
  KW_DriverCreateTextureFunction createTexture;
  KW_DriverDestroyTextureFunction destroyTexture;
  KW_DriverRenderTextureFunction renderTexture;
  KW_DriverCreateFontFunction createFont;
  KW_DriverDestroyFontFunction destoryFont;
  KW_DriverRenderTextFunction renderText;
  KW_DriverUpdateFunction update;
  void * priv;
};

#ifdef __cplusplus
};
#endif

#endif //KIWI_DRIVER_H
