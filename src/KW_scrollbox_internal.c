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
  SDL_Rect targetgeom;
  SDL_Renderer * renderer;
  SDL_Texture * tileset;
  
  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);
  
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
