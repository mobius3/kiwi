#ifndef GUI_LABEL_INTERNAL_H
#define GUI_LABEL_INTERNAL_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "GUI_label.h"

typedef
struct GUI_Label {
  char * text;
  SDL_Texture * textrender;
  SDL_Color color;
  int style;
  TTF_Font * font;
  GUI_LabelHorizontalAlignment halign;
  GUI_LabelVerticalAlignment valign;
  int hoffset;
  int voffset;
} GUI_Label;

#endif