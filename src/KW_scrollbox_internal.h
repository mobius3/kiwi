#ifndef KW_SCROLLBOX_INTERNAL_H
#define KW_SCROLLBOX_INTERNAL_H

#include "KW_widget.h"

struct KW_Scrollbox {
  KW_Widget * root;
};

void PaintScrollboxFrame(KW_Widget * widget);
void DestroyScrollboxFrame(KW_Widget * widget);
void ScrollboxAreaKeyUp(KW_Widget * widget, SDL_Keycode sym, SDL_Scancode code);

#endif
