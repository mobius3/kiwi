#include "KW_scrollbox_internal.h"
#include "KW_widget.h"
#include "KW_scrollbox.h"
#include "KW_gui.h"
#include "KW_tilerenderer.h"


KW_Scrollbox * AllocScrollbox() {
  KW_Scrollbox * scrollbox = calloc(sizeof(*scrollbox), 1);
  return scrollbox;
}

static int imin(int i, int j) {
  return (i < j) ? i : j;
}

static int imax(int i, int j) {
  return (i < j) ? j : i;
}

void RenderScrollboxFrame(KW_Scrollbox * sb) {
  KW_Surface * tileset;
  KW_Rect targetgeom;
  KW_Widget * widget = sb->root;
  tileset = KW_GetWidgetTilesetSurface(widget);
  KW_GetWidgetGeometry(widget, &targetgeom);
  
  if (sb->framerender != NULL) KW_ReleaseTexture(KW_GetWidgetRenderer(widget), sb->framerender);
  sb->framerender = KW_CreateTileFrameTexture(KW_GetWidgetRenderer(widget), tileset, 9, 0, targetgeom.w, targetgeom.h, KW_FALSE, KW_FALSE);
}

void PaintScrollboxFrame(KW_Widget * widget, const KW_Rect * absolute, void * data) {
  KW_Rect targetgeom = *absolute, innergeom, outergeom, boxgeom;
  KW_Rect scrollgeom, rootgeom;
  float innerh, innerw;
  KW_Scrollbox * sb = (KW_Scrollbox *) data;
  KW_GetWidgetGeometry(widget, &boxgeom);
  KW_GetWidgetGeometry(sb->outer, &outergeom);
  KW_GetWidgetGeometry(sb->root, &rootgeom);
  
  /* check if inner rectancle changed size */
  KW_GetWidgetComposedGeometry(sb->inner, &innergeom);
  if (sb->innercomposite.x != innergeom.x ||
      sb->innercomposite.y != innergeom.y || 
      sb->innercomposite.w != innergeom.w || 
      sb->innercomposite.h != innergeom.h) 
  {
    /* vertical scroll stuff */
    innerh = innergeom.h;
    KW_GetWidgetGeometry(sb->vscroll, &scrollgeom);
    scrollgeom.x = imax(boxgeom.w - TILESIZE * 3, TILESIZE);
    scrollgeom.w = imin(TILESIZE *2, boxgeom.w - TILESIZE);
    scrollgeom.h = (unsigned)((outergeom.h / innerh) * outergeom.h);
    scrollgeom.y = (unsigned)((-innergeom.y / innerh) * outergeom.h + TILESIZE);
    innergeom.h = (unsigned)innerh;
    KW_SetWidgetGeometry(sb->vscroll, &scrollgeom);
    
    /* horizontal scroll stuff */
    innerw = innergeom.w;
    KW_GetWidgetGeometry(sb->hscroll, &scrollgeom);
    scrollgeom.y = imax(boxgeom.h - TILESIZE * 3, TILESIZE);
    scrollgeom.w = (unsigned)((outergeom.w / innerw) * outergeom.w);
    scrollgeom.h = imin(TILESIZE * 2, boxgeom.h - TILESIZE);
    scrollgeom.x = (unsigned)((-innergeom.x / innerw) * outergeom.w + TILESIZE);
    innergeom.w = (unsigned)innerw;
    KW_SetWidgetGeometry(sb->hscroll, &scrollgeom);
    sb->innercomposite = innergeom;
  }
  
  KW_ScrollboxVerticalScroll(sb->root, 0);
  KW_ScrollboxHorizontalScroll(sb->root, 0);

  if (KW_QueryWidgetHint(sb->root, KW_WIDGETHINT_ALLOWTILESTRETCH)) {
    KW_RenderTileFrame(KW_GetWidgetRenderer(sb->root), KW_GetWidgetTilesetTexture(sb->root), 9, 0, &targetgeom, KW_TRUE, KW_TRUE);
  } else {
    if (!sb->framerender) RenderScrollboxFrame(sb);
    KW_RenderCopy(KW_GetWidgetRenderer(widget), sb->framerender, NULL, &targetgeom);
  }
}

void VerticalBarDrag(KW_Widget * widget, int x, int y, int xrel, int yrel) {
  KW_Widget * root = KW_GetWidgetParent(widget);
  KW_Scrollbox * sb = KW_GetWidgetData(root, PaintScrollboxFrame);
  KW_Rect outergeom;
  (void)x; (void) y; (void) xrel;
  KW_GetWidgetGeometry(sb->outer, &outergeom);
  KW_ScrollboxVerticalScroll(sb->root, (int)(-yrel * (sb->innercomposite.h / outergeom.h * 1.0f)));
}

void ChildrenChange(KW_Widget * widget, KW_WidgetChildrenChangeEvent what, KW_Widget * children) {
  /* nothing else should be on the root */
  if (what == KW_CHILDRENCHANGE_ADDED) {
    KW_Scrollbox * sb = KW_GetWidgetData(widget, PaintScrollboxFrame);
    KW_Rect geom;
    KW_ReparentWidget(children, sb->inner);
    KW_GetWidgetComposedGeometry(sb->inner, &geom);
    KW_SetWidgetGeometry(sb->inner, &geom);
  }
}


void RootScrollboxGeometryChange(KW_Widget * widget, const KW_Rect * geometry, const KW_Rect * oldgeom) {
  KW_Scrollbox * sb = KW_GetWidgetData(widget, PaintScrollboxFrame);
  KW_Rect rootgeom, areageom;
  rootgeom = *geometry;
  
  /* configure outer geometry */
  areageom.w = rootgeom.w - TILESIZE * 4;
  areageom.h = rootgeom.h - TILESIZE * 4;
  areageom.x = TILESIZE; areageom.y = TILESIZE;
  KW_SetWidgetGeometry(sb->outer, &areageom);
  
  /* configure inner geometry without changing x/y */
  KW_GetWidgetGeometry(sb->inner, &areageom);
  areageom.w = rootgeom.w - TILESIZE * 4;
  areageom.h = rootgeom.h - TILESIZE * 4;
  KW_SetWidgetGeometry(sb->inner, &areageom);
  sb->innercomposite.x = sb->innercomposite.y = sb->innercomposite.w = sb->innercomposite.h = 0;

  if (geometry->w != oldgeom->w || geometry->h != oldgeom->h) {
    if (!KW_QueryWidgetHint(sb->root, KW_WIDGETHINT_ALLOWTILESTRETCH))
      RenderScrollboxFrame(sb);
  }
}


void HorizontalBarDrag(KW_Widget * widget, int x, int y, int xrel, int yrel) {
  KW_Widget * root = KW_GetWidgetParent(widget);
  KW_Scrollbox * sb = KW_GetWidgetData(root, PaintScrollboxFrame);
  KW_Rect outergeom;
  (void) yrel; (void) xrel; (void) x; (void) y;
  KW_GetWidgetGeometry(sb->outer, &outergeom);
  KW_ScrollboxHorizontalScroll(sb->root, (int)(-xrel * (sb->innercomposite.w / outergeom.w * 1.0f)));
}


void ScrollboxKeyUp(KW_Widget * widget, SDL_Keycode sym, SDL_Scancode code) {
  KW_Scrollbox * scrollbox = KW_GetWidgetData(widget, PaintScrollboxFrame);
  (void) sym;
  if (scrollbox == NULL)
    scrollbox = KW_GetWidgetData(widget, PaintScrollboxFrame);
  switch (code) {
    case SDL_SCANCODE_UP:
      KW_ScrollboxVerticalScroll(scrollbox->root, -5);
      break;
    case SDL_SCANCODE_DOWN:
      KW_ScrollboxVerticalScroll(scrollbox->root, 5);
      break;
    default:
      break;
  }
}

void DestroyScrollboxFrame(KW_Widget * widget) {
  KW_Scrollbox * sb = KW_GetWidgetData(widget, PaintScrollboxFrame);
  free(sb);
}
