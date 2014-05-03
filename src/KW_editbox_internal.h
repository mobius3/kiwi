#ifndef KW_EDITBOX_INTERNAL_H
#define KW_EDITBOX_INTERNAL_H

#include "SDL.h"

typedef struct KW_Edtibox {
  KW_Widget * labelwidget;
  char text[1024];
  unsigned int cursor;
  int cursormove;
  SDL_TimerID cursortimer;
  SDL_bool mouseover;
  SDL_bool clicked;
  SDL_bool active;
} KW_Editbox;

#endif