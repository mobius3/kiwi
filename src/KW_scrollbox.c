#include "KW_scrollbox.h"
#include "KW_scrollbox_internal.h"
#include "KW_tilerenderer.h"
#include "KW_button.h"

KW_Widget * KW_CreateScrollbox(KW_GUI * gui, KW_Widget * parent, const KW_Rect * geometry) {
  /* creates root scrollbox widget */
  KW_Scrollbox * scrollbox = AllocScrollbox();
  KW_Widget * outer, * inner;
  KW_Widget * root = KW_CreateWidget(gui, parent, geometry, PaintScrollboxFrame, DestroyScrollboxFrame, scrollbox);
  
  KW_Rect areageom = *geometry;
  KW_Rect buttongeom;
  /* Ignore borders */
  areageom.x = TILESIZE;
  areageom.y = TILESIZE;
  areageom.w = geometry->w - TILESIZE * 4;
  areageom.h = geometry->h - TILESIZE * 4;
  outer = KW_CreateWidget(gui, root, &areageom, NULL, NULL, scrollbox);
  areageom.x = 0; areageom.y = 0; /* areageom.h = 0; areageom.w = 0; */
  inner = KW_CreateWidget(gui, outer, &areageom, NULL, NULL, scrollbox);
  KW_SetWidgetGeometry(inner, &areageom);
  KW_SetClipChildrenWidgets(outer, KW_TRUE);
  KW_AddWidgetKeyUpHandler(root, ScrollboxKeyUp);
  KW_AddWidgetKeyUpHandler(inner, ScrollboxKeyUp);
  
  scrollbox->outer = outer;
  scrollbox->inner = inner;
  scrollbox->root = root;
  
  buttongeom.x = geometry->w - TILESIZE * 3;
  buttongeom.y = geometry->y + TILESIZE;
  buttongeom.w = TILESIZE*2;
  buttongeom.h = TILESIZE*2;
  scrollbox->vscroll = KW_CreateButton(gui, root, NULL, &buttongeom);
  
  buttongeom.x = TILESIZE;
  buttongeom.y = geometry->h - TILESIZE * 3;
  buttongeom.w = TILESIZE*2;
  buttongeom.h = TILESIZE*2;
  scrollbox->hscroll = KW_CreateButton(gui, root, NULL, &buttongeom);
  
  KW_AddWidgetChildrenChangeHandler(root, ChildrenChange);

  KW_AddWidgetDragHandler(scrollbox->vscroll, VerticalBarDrag);  
  KW_AddWidgetDragHandler(scrollbox->hscroll, HorizontalBarDrag);  
  
  KW_AddWidgetGeometryChangeHandler(root, RootScrollboxGeometryChange);
  KW_EnableWidgetHint(scrollbox->root, KW_WIDGETHINT_ALLOWTILESTRETCH, KW_FALSE);
  RenderScrollboxFrame(scrollbox);
  return scrollbox->root;
}

void KW_ScrollboxVerticalScroll(KW_Widget * scrollbox, int amount) {
  /* remember we are dealing with the *outer* widget */
  KW_Rect geom, outer;
  KW_Scrollbox * sb = KW_GetWidgetData(scrollbox, PaintScrollboxFrame);
  KW_GetWidgetGeometry(sb->inner, &geom);
  KW_GetWidgetGeometry(sb->outer, &outer);
  
  if (sb->innercomposite.y + amount > 0) amount = -sb->innercomposite.y;
  if (sb->innercomposite.y + sb->innercomposite.h + amount < outer.h)
    amount = outer.h - (sb->innercomposite.y + sb->innercomposite.h);
  
  geom.y += amount;
  KW_SetWidgetGeometry(sb->inner, &geom);
}

void KW_ScrollboxHorizontalScroll(KW_Widget * scrollbox, int amount) {
  /* remember we are dealing with the *outer* widget */
  KW_Rect geom, outer;
  KW_Scrollbox * sb = KW_GetWidgetData(scrollbox, PaintScrollboxFrame);
  KW_GetWidgetGeometry(sb->inner, &geom);
  KW_GetWidgetGeometry(sb->outer, &outer);
  if (sb->innercomposite.x + amount > 0) amount = -sb->innercomposite.x;
  if (sb->innercomposite.x + sb->innercomposite.w + amount < outer.w)
    amount = outer.w - (sb->innercomposite.x + sb->innercomposite.w);
  geom.x += amount;
  KW_SetWidgetGeometry(sb->inner, &geom);
}
