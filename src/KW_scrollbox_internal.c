#include "KW_scrollbox_internal.h"
#include "KW_widget.h"
#include "KW_scrollbox.h"
#include "KW_gui.h"
#include "KW_tilerenderer.h"


KW_Scrollbox * AllocScrollbox() {
  KW_Scrollbox * scrollbox = calloc(sizeof(*scrollbox), 1);
  return scrollbox;
}

void PaintScrollboxFrame(KW_Widget * widget) {
  SDL_Rect targetgeom, innergeom, outergeom, boxgeom;
  SDL_Rect scrollgeom;
  SDL_Renderer * renderer;
  SDL_Texture * tileset;
  float innerh, innerw, diff;
  KW_Scrollbox * sb = KW_GetWidgetData(widget, KW_WIDGETTYPE_SCROLLBOX);
  
  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);
  KW_GetWidgetGeometry(widget, &boxgeom);
  KW_GetWidgetGeometry(sb->outer, &outergeom);
  
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
    scrollgeom.x = boxgeom.w - TILESIZE * 3;
    scrollgeom.w = TILESIZE *2;
    scrollgeom.h = (outergeom.h / innerh) * outergeom.h;
    scrollgeom.y = (-innergeom.y / innerh) * outergeom.h + TILESIZE;
    /* underflow */
    if (innergeom.y + innerh < outergeom.h) {
      diff = (outergeom.h - (innergeom.y + innerh));
      innerh += diff;
      scrollgeom.h = (outergeom.h / innerh) * outergeom.h;
      scrollgeom.y = (-innergeom.y / innerh) * outergeom.h + TILESIZE;
    }
    innergeom.h = innerh;
    KW_SetWidgetGeometry(sb->vscroll, &scrollgeom);
    
    /* horizontal scroll stuff */
    innerw = innergeom.w;
    KW_GetWidgetGeometry(sb->hscroll, &scrollgeom);
    scrollgeom.y = boxgeom.h - TILESIZE * 3;
    scrollgeom.w = (outergeom.w / innerw) * outergeom.w;
    scrollgeom.h = TILESIZE * 2;
    scrollgeom.x = (-innergeom.x / innerw) * outergeom.w + TILESIZE;
    
    if (innergeom.x + innerw < outergeom.w) {
      diff = (outergeom.w - (innergeom.x + innerw));
      innerw += diff;
      scrollgeom.w = SDL_ceil((outergeom.w / innerw) * outergeom.w);
      scrollgeom.x = (-innergeom.x / innerw) * outergeom.w + TILESIZE;
    }
    innergeom.w = innerw;
    KW_SetWidgetGeometry(sb->hscroll, &scrollgeom);
    
    sb->innercomposite = innergeom;
  }
  
  KW_ScrollboxVerticalScroll(sb->root, 0);
  KW_ScrollboxHorizontalScroll(sb->root, 0);
  
  
  renderer = KW_GetWidgetRenderer(widget);
  tileset = KW_GetTilesetTexture(KW_GetGUI(widget));
  
  /* render bg frame */
  KW_RenderTileFrame(renderer, tileset, 9, 0, targetgeom.x, targetgeom.y, targetgeom.w, targetgeom.h);
}

void VerticalBarDrag(KW_Widget * widget, int x, int y, int xrel, int yrel) {
  KW_Widget * root = KW_GetWidgetParent(widget);
  KW_Scrollbox * sb = KW_GetWidgetData(root, KW_WIDGETTYPE_SCROLLBOX);
  SDL_Rect outergeom;
  KW_GetWidgetGeometry(sb->outer, &outergeom);
  KW_ScrollboxVerticalScroll(sb->root, -yrel * (sb->innercomposite.h / outergeom.h * 1.0f));
}

void ChildrenChange(KW_Widget * widget, KW_WidgetChildrenChangeEvent what, KW_Widget * children) {
  /* nothing else should be on the root */
  if (what == KW_CHILDRENCHANGE_ADDED) {
    KW_Scrollbox * sb = KW_GetWidgetData(widget, KW_WIDGETTYPE_SCROLLBOX);
    KW_ReparentWidget(children, sb->inner);
  }
}


void RootScrollboxGeometryChange(KW_Widget * widget, const SDL_Rect * geometry, const SDL_Rect * oldgeom) {
  KW_Scrollbox * sb = KW_GetWidgetData(widget, KW_WIDGETTYPE_SCROLLBOX);
  SDL_Rect rootgeom, areageom;
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
}


void HorizontalBarDrag(KW_Widget * widget, int x, int y, int xrel, int yrel) {
  KW_Widget * root = KW_GetWidgetParent(widget);
  KW_Scrollbox * sb = KW_GetWidgetData(root, KW_WIDGETTYPE_SCROLLBOX);
  SDL_Rect outergeom;
  KW_GetWidgetGeometry(sb->outer, &outergeom);
  KW_ScrollboxHorizontalScroll(sb->root, -xrel * (sb->innercomposite.w / outergeom.w * 1.0f));
}


void DestroyScrollboxFrame(KW_Widget * widget) {

}


void ScrollboxKeyUp(KW_Widget * widget, SDL_Keycode sym, SDL_Scancode code) {
  KW_Scrollbox * scrollbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_SCROLLBOX);
  if (scrollbox == NULL)
  scrollbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_NONE);
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
