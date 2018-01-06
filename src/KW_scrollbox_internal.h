#ifndef KW_SCROLLBOX_INTERNAL_H
#define KW_SCROLLBOX_INTERNAL_H

#include "KW_widget.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct KW_Scrollbox {
  KW_Widget   * root;
  KW_Widget   * outer;
  KW_Widget   * inner;
  KW_Rect      innercomposite;
  KW_Texture * framerender;
  int           x;
  int           y;
  KW_Widget   * vscroll;
  KW_Widget   * hscroll;
} KW_Scrollbox;

KW_Scrollbox * AllocScrollbox();
void PaintScrollboxFrame(KW_Widget * widget, const KW_Rect * absolute, void * data);
void DestroyScrollboxFrame(KW_Widget * widget);
void ScrollboxKeyUp(KW_Widget * widget, SDL_Keycode sym, SDL_Scancode code);
void VerticalBarDrag(KW_Widget * widget, int x, int y, int xrel, int yrel);
void HorizontalBarDrag(KW_Widget * widget, int x, int y, int xrel, int yrel);
void ChildrenChange(KW_Widget * widget, KW_WidgetChildrenChangeEvent what, KW_Widget * children);
void RootScrollboxGeometryChange(KW_Widget * widget, const KW_Rect * newgeom, const KW_Rect * oldgeom);
void RenderScrollboxFrame(KW_Scrollbox * sb);
#ifdef __cplusplus
}
#endif

#endif
