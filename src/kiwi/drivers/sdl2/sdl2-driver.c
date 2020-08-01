#include "kiwi/drivers/sdl2/sdl2-driver.h"
#include "sdl2-driver-internal.h"

#include "kiwi/core/driver.h"
#include "sdl2-memory.h"
#include "sdl2-texture.h"
#include "sdl2-font.h"

void KW_Rect2Rect(KW_Rect const * src, SDL_Rect * dst) {
  dst->x = src->x;
  dst->y = src->y;
  dst->w = src->w;
  dst->h = src->h;
}

KW_Driver * KWSDL2_CreateDriver(SDL_Window * window) {
  SDL_Renderer * renderer = SDL_GetRenderer(window);
  KWSDL2_Driver * sdl2 = SDL_malloc(sizeof(*sdl2));
  KW_Driver * driver = SDL_malloc(sizeof(*driver));
  sdl2->renderer = renderer;
  sdl2->window = window;
  driver->priv = sdl2;
  driver->allocate = KWSDL2_Allocate;
  driver->free = KWSDL2_Free;
  driver->createTexture = KWSDL2_CreateTexture;
  driver->destroyTexture = KWSDL2_DestroyTexture;
  driver->renderTexture = KWSDL2_RenderTexture;
  driver->createFont = KWSDL2_CreateFont;
  driver->destoryFont = KWSDL2_DestroyFont;
  driver->renderText = KWSDL2_RenderText;
  return driver;
}
