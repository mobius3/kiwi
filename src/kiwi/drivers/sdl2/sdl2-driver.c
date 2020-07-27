#include "kiwi/drivers/sdl2/sdl2-driver.h"
#include "kiwi/core/driver.h"
#include "sdl2-memory.h"


struct KWSDL2_Driver {
  SDL_Window * window;
  SDL_Renderer * renderer;
};

typedef struct KWSDL2_Driver KWSDL2_Driver;

KW_Driver * KWSDL2_CreateDriver(SDL_Window * window) {
  SDL_Renderer * renderer = SDL_GetRenderer(window);
  KWSDL2_Driver * sdl2 = SDL_malloc(sizeof(*sdl2));
  KW_Driver * driver = SDL_malloc(sizeof(*driver));
  sdl2->renderer = renderer;
  sdl2->window = window;
  driver->priv = sdl2;
  driver->allocate = KWSDL2_Allocate;
  driver->free = KWSDL2_Free;
  return driver;
}
