#include "GUI_button.h"
#include "GUI_button_internal.h"
#include "GUI_label.h"
#include "GUI_tilerenderer.h"
#include "GUI_gui.h"

GUI_Button * AllocButton();
void PaintButton(GUI_Widget * widget);
void DestroyButton(GUI_Widget * widget);


GUI_Widget * GUI_CreateButton(GUI_GUI * gui, GUI_Widget * parent, const char * text, const SDL_Rect * geometry) {
  SDL_Rect labelgeom;
  labelgeom.x = TILESIZE;
  labelgeom.y = TILESIZE;
  labelgeom.w = geometry->w - TILESIZE * 2;
  labelgeom.h = geometry->h - TILESIZE * 2;
  
  GUI_Button * button = AllocButton();
  GUI_Widget * widget = GUI_CreateWidget(gui, parent, GUI_WIDGETTYPE_BUTTON, geometry, PaintButton, DestroyButton, button);
  button->labelwidget = GUI_CreateLabel(gui, widget, text, &labelgeom);
  return widget;
}

/** internal stuff **/

void PaintButton(GUI_Widget * widget) {
  SDL_Rect targetgeom;
  SDL_Renderer * renderer;
  SDL_Texture * tileset;
  
  GUI_GetWidgetAbsoluteGeometry(widget, &targetgeom);
  
  renderer = GUI_GetWidgetRenderer(widget);
  tileset = GUI_GetTileset(GUI_GetGUI(widget));
  
  /* fill with background */
  GUI_RenderTileFill(renderer, tileset, 4, 1, targetgeom.x + TILESIZE, targetgeom.y + TILESIZE, targetgeom.w - TILESIZE * 2, targetgeom.h - TILESIZE * 2);
  
  /* fill top */
  GUI_RenderTileFill(renderer, tileset, 4, 0, targetgeom.x + TILESIZE, targetgeom.y, targetgeom.w - TILESIZE * 2, TILESIZE);
  
  /* fill bottom */
  GUI_RenderTileFill(renderer, tileset, 4, 2, targetgeom.x + TILESIZE, targetgeom.y + targetgeom.h - TILESIZE, targetgeom.w - TILESIZE * 2, TILESIZE);
  
  /* fill left */
  GUI_RenderTileFill(renderer, tileset, 3, 1, targetgeom.x, targetgeom.y + TILESIZE, TILESIZE, targetgeom.h - TILESIZE * 2);
  
  /* fill right */
  GUI_RenderTileFill(renderer, tileset, 5, 1, targetgeom.x + (targetgeom.w - TILESIZE), targetgeom.y + TILESIZE, TILESIZE, targetgeom.h - TILESIZE * 2);
  
  /* render top left */
  GUI_RenderTile(renderer, tileset, 3, 0, targetgeom.x, targetgeom.y);
  
  /* render top right */
  GUI_RenderTile(renderer, tileset, 5, 0, targetgeom.x + targetgeom.w - TILESIZE, targetgeom.y);
  
  /* render bottom left */
  GUI_RenderTile(renderer, tileset, 3, 2, targetgeom.x, targetgeom.y + targetgeom.h - TILESIZE);
  
  
  /* render bottom right */
  GUI_RenderTile(renderer, tileset, 5, 2, targetgeom.x + (targetgeom.w - TILESIZE), targetgeom.y + targetgeom.h - TILESIZE);
}

void DestroyButton(GUI_Widget * widget) {
  
}


GUI_Button * AllocButton() {
  GUI_Button * button = calloc(sizeof(GUI_Button), 1);
  return button;
}
