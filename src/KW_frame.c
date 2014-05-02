#include "KW_gui.h"
#include "KW_frame.h"
#include "SDL.h"
#include "KW_tilerenderer.h"

void PaintFrame(KW_Widget * frame);

KW_Widget * KW_CreateFrame(KW_GUI * gui, KW_Widget * parent, const SDL_Rect * geometry) {
  KW_Widget * widget = KW_CreateWidget(gui, parent, KW_WIDGETTYPE_FRAME, geometry, PaintFrame, NULL, NULL);
  return widget;
}

void PaintFrame(KW_Widget * widget) {
  SDL_Rect targetgeom;
  SDL_Renderer * renderer;
  SDL_Texture * tileset;
  
  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);
  
  renderer = KW_GetWidgetRenderer(widget);
  tileset = KW_GetTileset(KW_GetGUI(widget));
  
  KW_RenderTileFrame(renderer, tileset, 0, 0, targetgeom.x, targetgeom.y, targetgeom.w, targetgeom.h);
}

