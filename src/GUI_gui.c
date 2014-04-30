#include "GUI_widget_internal.h"
#include "GUI_widget.h"
#include "GUI_gui.h"
#include "GUI_gui_internal.h"
#include "GUI_eventwatcher.h"

#include "SDL_ttf.h"



GUI_GUI * GUI_Init(SDL_Renderer * renderer, SDL_Texture * tileset) {
  struct GUI_GUI * gui = calloc(sizeof(*gui), 1);
  
  gui->tileset = tileset;
  gui->renderer = renderer;
  gui->rootwidget = AllocWidget();
  gui->rootwidget->gui = gui;
  
  SDL_AddEventWatch(GUI_EventWatcher, gui);
  
  return gui;
}



void GUI_SetRender(GUI_GUI * gui, SDL_Renderer * renderer) {
  gui->renderer = renderer;
}

void GUI_SetTileset(GUI_GUI * gui, SDL_Texture * tileset) {
  gui->tileset = tileset;
}


SDL_Renderer * GUI_GetRenderer(GUI_GUI * gui) {
  return gui->renderer;
}


void GUI_SetRenderer(GUI_GUI * gui, SDL_Renderer * renderer) {
  gui->renderer = renderer;
}


SDL_Texture * GUI_GetTileset(GUI_GUI * gui) {
  return gui->tileset;
}


void GUI_Quit(GUI_GUI * gui) {
  GUI_DestroyWidget(gui->rootwidget, 1);
  free(gui);
}

void GUI_SetFont(GUI_GUI * gui, TTF_Font * font) {
  if (font == NULL) return;
  gui->font = font;
  return;
}

TTF_Font * GUI_GetFont(GUI_GUI * gui) {
  return gui->font;
}


GUI_GUI * GUI_GetGUI(GUI_Widget * widget) {
  return GUI_GetWidgetGUI(widget);
}

void GUI_Paint(GUI_GUI * gui) {
  GUI_PaintWidget(gui->rootwidget);
}

