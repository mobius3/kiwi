#include "KW_scrollbox_internal.h"
#include "KW_widget.h"
#include "KW_scrollbox.h"
#include "KW_gui.h"
#include "KW_tilerenderer.h"

void PaintScrollboxFrame(KW_Widget * widget) {
  SDL_Rect targetgeom;
  SDL_Renderer * renderer;
  SDL_Texture * tileset;
  
  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);
  
  renderer = KW_GetWidgetRenderer(widget);
  tileset = KW_GetTileset(KW_GetGUI(widget));
  
  KW_RenderTileFrame(renderer, tileset, 9, 0, targetgeom.x, targetgeom.y, targetgeom.w, targetgeom.h);
}


void DestroyScrollboxFrame(KW_Widget * widget) {

}


void ScrollboxAreaKeyUp(KW_Widget * widget, SDL_Keycode sym, SDL_Scancode code) {
  switch (code) {
    case SDL_SCANCODE_UP:
      KW_ScrollboxVerticalScroll(widget, -5);
      break;
    case SDL_SCANCODE_DOWN:
      KW_ScrollboxVerticalScroll(widget, 5);
      break;
    default:
      break;
  }
}
