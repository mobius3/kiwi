#include "KW_scrollbox.h"
#include "KW_scrollbox_internal.h"
#include "KW_tilerenderer.h"

KW_Widget * KW_CreateScrollbox(KW_GUI * gui, KW_Widget * parent, const SDL_Rect * geometry) {
  /* creates root scrollbox widget */
  KW_Widget * area;
  KW_Widget * root = KW_CreateWidget(gui, parent, KW_WIDGETTYPE_SCROLLBOX,
                                         geometry, PaintScrollboxFrame,
                                         DestroyScrollboxFrame, NULL);
  
  SDL_Rect areageom = *geometry;
  /* Ignore borders */
  areageom.x = TILESIZE;
  areageom.y = TILESIZE;
  areageom.w = geometry->w - TILESIZE * 2;
  areageom.h = geometry->h - TILESIZE * 2;
  area = KW_CreateWidget(gui, root, KW_WIDGETTYPE_NONE, &areageom, NULL, NULL, NULL);
  KW_SetClipChildrenWidgets(area, SDL_TRUE);
  KW_AddWidgetKeyUpHandler(area, ScrollboxAreaKeyUp);
  
  /* everyone needs to be children of this area */
  return area;
}

void KW_ScrollboxVerticalScroll(KW_Widget * scrollbox, int amount) {
  /* remember we are dealing with the *area* widget */
  SDL_Rect geom;
  unsigned int i = 0, count;
  KW_Widget * const * children = KW_GetWidgetChildren(scrollbox, &count);  
  for (i = 0; i < count; i++) {
    KW_GetWidgetGeometry(children[i], &geom);
    geom.y += amount;
    KW_SetWidgetGeometry(children[i], &geom);
  }
}

void KW_ScrollboxHorizontalScroll(KW_Widget * scrollbox, int amount) {
  /* remember we are dealing with the *area* widget */
  SDL_Rect geom;
  unsigned int i = 0, count;
  KW_Widget * const * children = KW_GetWidgetChildren(scrollbox, &count);
  for (i = 0; i < count; i++) {
    KW_GetWidgetGeometry(children[i], &geom);
    geom.x += amount;
    KW_SetWidgetGeometry(children[i], &geom);
  }
}
