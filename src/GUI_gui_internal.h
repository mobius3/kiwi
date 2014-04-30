#ifndef GUI_GUI_INTERNAL_H
#define GUI_GUI_INTERNAL_H

#include "GUI_widget.h"
#include "SDL_ttf.h"

struct GUI_GUI {
  SDL_Texture * tileset;
  SDL_Renderer * renderer;
  GUI_Widget * rootwidget; /* the parent to all parent-less widgets :) */
  TTF_Font * font;
};

typedef struct GUI_GUI GUI_GUI;

#endif