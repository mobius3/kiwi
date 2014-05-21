#ifndef KW_SCROLLBOX_INTERNAL_H
#define KW_SCROLLBOX_INTERNAL_H

#include "KW_widget.h"

typedef struct KW_Scrollbox {
  KW_Widget * root;
  KW_Widget * outer;
  KW_Widget * inner;
} KW_Scrollbox;

KW_Scrollbox * AllocScrollbox();
void PaintScrollboxFrame(KW_Widget * widget);
void DestroyScrollboxFrame(KW_Widget * widget);
void ScrollboxKeyUp(KW_Widget * widget, SDL_Keycode sym, SDL_Scancode code);

#endif
