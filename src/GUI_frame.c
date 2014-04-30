#include "GUI_gui.h"
#include "GUI_frame.h"
#include "SDL.h"
#include "GUI_tilerenderer.h"

void PaintFrame(GUI_Widget * frame);

GUI_Widget * GUI_CreateFrame(GUI_GUI * gui, GUI_Widget * parent, const SDL_Rect * geometry) {
  GUI_Widget * widget = GUI_CreateWidget(gui, parent, GUI_WIDGETTYPE_FRAME, geometry, PaintFrame, NULL, NULL);
  return widget;
}

void PaintFrame(GUI_Widget * widget) {
  SDL_Rect targetgeom;
  SDL_Renderer * renderer;
  SDL_Texture * tileset;
  
  GUI_GetWidgetAbsoluteGeometry(widget, &targetgeom);
  
  renderer = GUI_GetWidgetRenderer(widget);
  tileset = GUI_GetTileset(GUI_GetGUI(widget));
  
  /* fill with background */
  GUI_RenderTileFill(renderer, tileset, 1, 1, targetgeom.x + TILESIZE, targetgeom.y + TILESIZE, targetgeom.w - TILESIZE * 2, targetgeom.h - TILESIZE * 2);
  
  /* fill top */
  GUI_RenderTileFill(renderer, tileset, 1, 0, targetgeom.x + TILESIZE, targetgeom.y, targetgeom.w - TILESIZE * 2, TILESIZE);
  
  /* fill bottom */
  GUI_RenderTileFill(renderer, tileset, 1, 2, targetgeom.x + TILESIZE, targetgeom.y + targetgeom.h - TILESIZE, targetgeom.w - TILESIZE * 2, TILESIZE);
  
  /* fill left */
  GUI_RenderTileFill(renderer, tileset, 0, 1, targetgeom.x, targetgeom.y + TILESIZE, TILESIZE, targetgeom.h - TILESIZE * 2);
  
  /* fill right */
  GUI_RenderTileFill(renderer, tileset, 2, 1, targetgeom.x + (targetgeom.w - TILESIZE), targetgeom.y + TILESIZE, TILESIZE, targetgeom.h - TILESIZE * 2);
  
  /* render top left */
  GUI_RenderTile(renderer, tileset, 0, 0, targetgeom.x, targetgeom.y);
  
  /* render top right */
  GUI_RenderTile(renderer, tileset, 2, 0, targetgeom.x + targetgeom.w - TILESIZE, targetgeom.y);
  
  /* render bottom left */
  GUI_RenderTile(renderer, tileset, 0, 2, targetgeom.x, targetgeom.y + targetgeom.h - TILESIZE);
  
  
  /* render bottom right */
  GUI_RenderTile(renderer, tileset, 2, 2, targetgeom.x + (targetgeom.w - TILESIZE), targetgeom.y + targetgeom.h - TILESIZE);
}

