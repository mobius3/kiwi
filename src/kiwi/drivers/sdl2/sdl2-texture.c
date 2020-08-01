#include "sdl2-texture.h"
#include "sdl2-driver-internal.h"
#include "SDL.h"

KW_Texture * KWSDL2_CreateTexture(KW_Driver * driver, const uint8_t * pixels, size_t width, size_t height, uint8_t bytesPerPixel) {
  KWSDL2_PREAMBLE
  KW_Texture * result = SDL_malloc(sizeof(*result));

  Uint32 rmask, gmask, bmask, amask;
  int x = (int) width, y = (int) height;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  SDL_Surface * surface = SDL_CreateRGBSurfaceFrom(
      (void *) pixels,
      x, y, bytesPerPixel * 8, x * bytesPerPixel, rmask, gmask, bmask, amask
  );
  SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);

  if (surface == NULL) {
    return NULL;
  }

  SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (texture != NULL) {
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  }
  SDL_FreeSurface(surface);
  result->priv = texture;
  return result;
}

void KWSDL2_DestroyTexture(KW_Driver * driver, KW_Texture * texture) {
  (void) driver;
  SDL_DestroyTexture(texture->priv);
}

void KWSDL2_RenderTexture(KW_Driver * driver, KW_Texture * texture, KW_Rect const * _src, KW_Rect const * _dst, KW_Rect const * _clip) {
  KWSDL2_PREAMBLE
  SDL_Texture * t = texture->priv;
  SDL_Rect src, dst, clip, oldclip;
  KW_Rect2Rect(_src, &src);
  KW_Rect2Rect(_dst, &dst);
  SDL_RenderGetClipRect(renderer, &oldclip);
  if (_clip) {
    KW_Rect2Rect(_clip, &clip);
    SDL_RenderSetClipRect(renderer, &clip);
  }

  SDL_RenderCopy(renderer, t, _src ? &src : NULL, _dst ? &dst : NULL);

  if (_clip) {
    SDL_RenderSetClipRect(renderer, &oldclip);
  }
}
