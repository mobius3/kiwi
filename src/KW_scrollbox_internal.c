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
  SDL_Rect targetgeom, innergeom, boxgeom;
  SDL_Rect vscrollgeom;
  SDL_Renderer * renderer;
  SDL_Texture * tileset;
  KW_Scrollbox * sb = KW_GetWidgetData(widget, KW_WIDGETTYPE_SCROLLBOX);
  
  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);
  KW_GetWidgetGeometry(widget, &boxgeom);
  
  /* check if inner rectancle changed size */
  KW_GetWidgetComposedGeometry(sb->inner, &innergeom);
  if (sb->innercomposite.x != innergeom.x ||
      sb->innercomposite.y != innergeom.y || 
      sb->innercomposite.w != innergeom.w || 
      sb->innercomposite.h != innergeom.h) 
  {
    sb->innercomposite = innergeom;
    KW_GetWidgetGeometry(sb->vscroll, &vscrollgeom);
    vscrollgeom.x = boxgeom.w - TILESIZE * 3;
    vscrollgeom.w = TILESIZE *2;
    vscrollgeom.h = (boxgeom.h / (innergeom.h * 1.0f)) * boxgeom.h - TILESIZE *2;
    vscrollgeom.y = innergeom.y / ((innergeom.h - boxgeom.h) * 1.0f);
    printf("vsg %f %d %d %d\n", innergeom.y / ((innergeom.h - boxgeom.h) * 1.0f), innergeom.y, innergeom.h, boxgeom.h);
    if (vscrollgeom.h > targetgeom.h  - TILESIZE *2) vscrollgeom.h = targetgeom.h - TILESIZE *2;
    KW_SetWidgetGeometry(sb->vscroll, &vscrollgeom);
  }
  
  
  renderer = KW_GetWidgetRenderer(widget);
  tileset = KW_GetTilesetTexture(KW_GetGUI(widget));
  
  /* render bg frame */
  KW_RenderTileFrame(renderer, tileset, 9, 0, targetgeom.x, targetgeom.y, targetgeom.w, targetgeom.h);
  
  /* render percent bar */
  KW_RenderTile(renderer, tileset, 9, 3, targetgeom.x + targetgeom.w - TILESIZE, targetgeom.y);
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
