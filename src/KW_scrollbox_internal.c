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
  SDL_Rect vscrollgeom;
  SDL_Renderer * renderer;
  SDL_Texture * tileset;
  float innerh, diff;
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
    innerh = innergeom.h;
    KW_GetWidgetGeometry(sb->vscroll, &vscrollgeom);
    vscrollgeom.x = boxgeom.w - TILESIZE * 3;
    vscrollgeom.w = TILESIZE *2;
    vscrollgeom.h = (outergeom.h / innerh) * outergeom.h;
    vscrollgeom.y = (-innergeom.y / innerh) * outergeom.h + TILESIZE;
    /* underflow */
    if (innergeom.y + innerh < outergeom.h) {
      diff = (outergeom.h - (innergeom.y + innerh));
      innerh += diff;
      vscrollgeom.h = (outergeom.h / innerh) * outergeom.h;
      vscrollgeom.y = (-innergeom.y / innerh) * outergeom.h + TILESIZE;
    }
    innergeom.h = innerh;
    sb->innercomposite = innergeom;
    KW_SetWidgetGeometry(sb->vscroll, &vscrollgeom);
  }
  
  KW_ScrollboxVerticalScroll(sb->inner, 0);
  
  
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
  KW_ScrollboxVerticalScroll(sb->inner, -yrel * (sb->innercomposite.h / outergeom.h * 1.0f));
}



void DestroyScrollboxFrame(KW_Widget * widget) {

}


void ScrollboxKeyUp(KW_Widget * widget, SDL_Keycode sym, SDL_Scancode code) {
  KW_Scrollbox * scrollbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_SCROLLBOX);
  if (scrollbox == NULL)
  scrollbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_NONE);
  switch (code) {
    case SDL_SCANCODE_UP:
      KW_ScrollboxVerticalScroll(scrollbox->outer, -5);
      break;
    case SDL_SCANCODE_DOWN:
      KW_ScrollboxVerticalScroll(scrollbox->outer, 5);
      break;
    default:
      break;
  }
}
