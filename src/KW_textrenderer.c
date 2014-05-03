#include "KW_textrenderer.h"


SDL_Texture * KW_RenderTextLine(TTF_Font * font, SDL_Renderer * renderer, const char * text, SDL_Color color, int styleflags) {
  if (font == NULL || text == NULL)
    return NULL;
  
  int previousstyle = TTF_GetFontStyle(font);
  TTF_SetFontStyle(font, styleflags);
  SDL_Surface * textsurface = TTF_RenderUTF8_Blended(font, text, color);
  SDL_Texture * ret = SDL_CreateTextureFromSurface(renderer, textsurface);
  SDL_FreeSurface(textsurface);
  TTF_SetFontStyle(font, previousstyle);
  return ret;
}
