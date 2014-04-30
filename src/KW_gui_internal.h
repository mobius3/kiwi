#ifndef KW_KW_INTERNAL_H
#define KW_KW_INTERNAL_H

#include "KW_widget.h"
#include "SDL_ttf.h"

struct KW_GUI {
  SDL_Texture * tileset;
  SDL_Renderer * renderer;
  KW_Widget * rootwidget; /* the parent to all parent-less widgets :) */
  TTF_Font * font;
  
  KW_Widget * currentmouseover; /* the current widget that has mouse over */
};

typedef struct KW_GUI KW_GUI;

#endif