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
  unsigned int cursor;
  SDL_bool showcursor;
  SDL_bool dirty;
  
  
  int textwidth;
  int textheight;
  int cursorx;
  int cursory;
  
  int x;
} KW_Label;

void PaintLabel(KW_Widget * widget);
void RenderLabelText(KW_Widget * widget);
void DestroyLabel(KW_Widget * widget);
void LabelFontChanged(KW_GUI * gui, void * data, TTF_Font * font);

#endif
