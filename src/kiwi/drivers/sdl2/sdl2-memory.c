#include "sdl2-memory.h"
#include "SDL.h"

void * KWSDL2_Allocate(KW_Driver * driver, size_t size) {
  (void) driver;
  return SDL_malloc(size);
}

void KWSDL2_Free(KW_Driver * driver, void * mem) {
  (void) driver;
  SDL_free(mem);
}