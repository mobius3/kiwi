#ifndef KW_LABEL_INTERNAL_H
#define KW_LABEL_INTERNAL_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "KW_label.h"

typedef
struct KW_Label {
  char * text;
  SDL_Texture * textrender;
  SDL_Color color;
  int style;
  TTF_Font * font;
  KW_LabelHorizontalAlignment halign;
  KW_LabelVerticalAlignment valign;
  int hoffset;
  int voffset;
} KW_Label;

#endif