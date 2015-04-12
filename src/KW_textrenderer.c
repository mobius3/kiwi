#include "KW_textrenderer.h"


SDL_Texture * KW_RenderTextLine(TTF_Font * font, SDL_Renderer * renderer, const char * text, SDL_Color color, int styleflags) {
  int previousstyle;
  SDL_Surface * textsurface;
  SDL_Texture * ret;
  if (font == NULL || text == NULL)
    return NULL;
  
  previousstyle = TTF_GetFontStyle(font);
  TTF_SetFontStyle(font, styleflags);
  textsurface = TTF_RenderUTF8_Blended(font, text, color);
  ret = SDL_CreateTextureFromSurface(renderer, textsurface);
  SDL_FreeSurface(textsurface);
  TTF_SetFontStyle(font, previousstyle);
  return ret;
}
