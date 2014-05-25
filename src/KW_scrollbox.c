#include "KW_scrollbox.h"
#include "KW_scrollbox_internal.h"
#include "KW_tilerenderer.h"
#include "KW_button.h"

KW_Widget * KW_CreateScrollbox(KW_GUI * gui, KW_Widget * parent, const SDL_Rect * geometry) {
  /* creates root scrollbox widget */
  KW_Scrollbox * scrollbox = AllocScrollbox();
  KW_Widget * outer, * inner, * button;
  KW_Widget * root = KW_CreateWidget(gui, parent, KW_WIDGETTYPE_SCROLLBOX,
                                         geometry, PaintScrollboxFrame,
                                         DestroyScrollboxFrame, scrollbox);
  
  SDL_Rect areageom = *geometry;
  SDL_Rect buttongeom;
  /* Ignore borders */
  areageom.x = TILESIZE;
  areageom.y = TILESIZE;
  areageom.w = geometry->w - TILESIZE * 4;
  areageom.h = geometry->h - TILESIZE * 2;
  outer = KW_CreateWidget(gui, root, KW_WIDGETTYPE_NONE, &areageom, PaintScrollboxFrame, NULL, scrollbox);
  areageom.x = 0; areageom.y = 0; areageom.h = areageom.w; areageom.w = areageom.h;
  inner = KW_CreateWidget(gui, outer, KW_WIDGETTYPE_NONE, &areageom, NULL, NULL, scrollbox);
  KW_SetClipChildrenWidgets(outer, SDL_TRUE);
  KW_AddWidgetKeyUpHandler(root, ScrollboxKeyUp);
  KW_AddWidgetKeyUpHandler(inner, ScrollboxKeyUp);
  scrollbox->outer = outer;
  scrollbox->inner = inner;
  scrollbox->root = root;
  
  buttongeom.x = geometry->w - TILESIZE * 3;
  buttongeom.y = geometry->y + TILESIZE;
  buttongeom.w = TILESIZE*2;
  buttongeom.h = TILESIZE*2;
  button = KW_CreateButton(gui, root, "", &buttongeom);
  /* everyone needs to be children of this area */
  return inner;
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
