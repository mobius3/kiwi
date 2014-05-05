#ifndef KW_EDITBOX_INTERNAL_H
#define KW_EDITBOX_INTERNAL_H

#include "SDL.h"
#include "KW_widget.h"

typedef struct KW_Edtibox {
  KW_Widget * widget;         // the widget we pertain to
  
  char text[1024];            // the actual text
  SDL_Texture * textrender;   // the rendered text
  TTF_Font * font;            // font used to render text
  unsigned int cursor;        // in which char index is the cursor
  SDL_bool mouseover;         // is mouse over the editbox
  SDL_bool clicked;           // was the editbox clicked
  SDL_bool active;            // is the editbox active
                             
  int textwidth;              // total width of the rendered text
  int textheight;             // total height of the rendered text
  int cursorx;                // cursor x position inside text texture
  int cursoradjustx;           // ajudstment in src.x text texture
  SDL_Color color;            // color used to render the text
  
} KW_Editbox;

#endif
