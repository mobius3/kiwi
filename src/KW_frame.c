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
  
  /* fill with background */
  KW_RenderTileFill(renderer, tileset, 1, 1, targetgeom.x + TILESIZE, targetgeom.y + TILESIZE, targetgeom.w - TILESIZE * 2, targetgeom.h - TILESIZE * 2);
  
  /* fill top */
  KW_RenderTileFill(renderer, tileset, 1, 0, targetgeom.x + TILESIZE, targetgeom.y, targetgeom.w - TILESIZE * 2, TILESIZE);
  
  /* fill bottom */
  KW_RenderTileFill(renderer, tileset, 1, 2, targetgeom.x + TILESIZE, targetgeom.y + targetgeom.h - TILESIZE, targetgeom.w - TILESIZE * 2, TILESIZE);
  
  /* fill left */
  KW_RenderTileFill(renderer, tileset, 0, 1, targetgeom.x, targetgeom.y + TILESIZE, TILESIZE, targetgeom.h - TILESIZE * 2);
  
  /* fill right */
  KW_RenderTileFill(renderer, tileset, 2, 1, targetgeom.x + (targetgeom.w - TILESIZE), targetgeom.y + TILESIZE, TILESIZE, targetgeom.h - TILESIZE * 2);
  
  /* render top left */
  KW_RenderTile(renderer, tileset, 0, 0, targetgeom.x, targetgeom.y);
  
  /* render top right */
  KW_RenderTile(renderer, tileset, 2, 0, targetgeom.x + targetgeom.w - TILESIZE, targetgeom.y);
  
  /* render bottom left */
  KW_RenderTile(renderer, tileset, 0, 2, targetgeom.x, targetgeom.y + targetgeom.h - TILESIZE);
  
  
  /* render bottom right */
  KW_RenderTile(renderer, tileset, 2, 2, targetgeom.x + (targetgeom.w - TILESIZE), targetgeom.y + targetgeom.h - TILESIZE);
}

