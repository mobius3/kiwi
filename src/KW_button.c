#include "KW_button.h"
#include "KW_button_internal.h"
#include "KW_label.h"
#include "KW_tilerenderer.h"
#include "KW_gui.h"

KW_Button * AllocButton();
void PaintButton(KW_Widget * widget);
void DestroyButton(KW_Widget * widget);


KW_Widget * KW_CreateButton(KW_GUI * gui, KW_Widget * parent, const char * text, const SDL_Rect * geometry) {
  SDL_Rect labelgeom;
  labelgeom.x = TILESIZE;
  labelgeom.y = TILESIZE;
  labelgeom.w = geometry->w - TILESIZE * 2;
  labelgeom.h = geometry->h - TILESIZE * 2;
  
  KW_Button * button = AllocButton();
  KW_Widget * widget = KW_CreateWidget(gui, parent, KW_WIDGETTYPE_BUTTON, geometry, PaintButton, DestroyButton, button);
  button->labelwidget = KW_CreateLabel(gui, widget, text, &labelgeom);
  KW_BlockWidgetInputEvents(button->labelwidget);
  return widget;
}

/** internal stuff **/

void PaintButton(KW_Widget * widget) {
  SDL_Rect targetgeom;
  SDL_Renderer * renderer;
  SDL_Texture * tileset;
  
  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);
  
  renderer = KW_GetWidgetRenderer(widget);
  tileset = KW_GetTileset(KW_GetGUI(widget));
  
  /* fill with background */
  KW_RenderTileFill(renderer, tileset, 4, 1, targetgeom.x + TILESIZE, targetgeom.y + TILESIZE, targetgeom.w - TILESIZE * 2, targetgeom.h - TILESIZE * 2);
  
  /* fill top */
  KW_RenderTileFill(renderer, tileset, 4, 0, targetgeom.x + TILESIZE, targetgeom.y, targetgeom.w - TILESIZE * 2, TILESIZE);
  
  /* fill bottom */
  KW_RenderTileFill(renderer, tileset, 4, 2, targetgeom.x + TILESIZE, targetgeom.y + targetgeom.h - TILESIZE, targetgeom.w - TILESIZE * 2, TILESIZE);
  
  /* fill left */
  KW_RenderTileFill(renderer, tileset, 3, 1, targetgeom.x, targetgeom.y + TILESIZE, TILESIZE, targetgeom.h - TILESIZE * 2);
  
  /* fill right */
  KW_RenderTileFill(renderer, tileset, 5, 1, targetgeom.x + (targetgeom.w - TILESIZE), targetgeom.y + TILESIZE, TILESIZE, targetgeom.h - TILESIZE * 2);
  
  /* render top left */
  KW_RenderTile(renderer, tileset, 3, 0, targetgeom.x, targetgeom.y);
  
  /* render top right */
  KW_RenderTile(renderer, tileset, 5, 0, targetgeom.x + targetgeom.w - TILESIZE, targetgeom.y);
  
  /* render bottom left */
  KW_RenderTile(renderer, tileset, 3, 2, targetgeom.x, targetgeom.y + targetgeom.h - TILESIZE);
  
  
  /* render bottom right */
  KW_RenderTile(renderer, tileset, 5, 2, targetgeom.x + (targetgeom.w - TILESIZE), targetgeom.y + targetgeom.h - TILESIZE);
}

void DestroyButton(KW_Widget * widget) {
  
}


KW_Button * AllocButton() {
  KW_Button * button = calloc(sizeof(KW_Button), 1);
  return button;
}
