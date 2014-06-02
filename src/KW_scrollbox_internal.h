#ifndef KW_SCROLLBOX_INTERNAL_H
#define KW_SCROLLBOX_INTERNAL_H

#include "KW_widget.h"

typedef struct KW_Scrollbox {
  KW_Widget * root;
  KW_Widget * outer;
  KW_Widget * inner;
  SDL_Rect innercomposite;
  int         x;
  int         y;
  
  KW_Widget * vscroll;
  KW_Widget * hscroll;
} KW_Scrollbox;

KW_Scrollbox * AllocScrollbox();
void PaintScrollboxFrame(KW_Widget * widget);
void DestroyScrollboxFrame(KW_Widget * widget);
void ScrollboxKeyUp(KW_Widget * widget, SDL_Keycode sym, SDL_Scancode code);
void VerticalBarDrag(KW_Widget * widget, int x, int y, int xrel, int yrel);

#endif
