#include "KW_widget_internal.h"
#include "KW_widget.h"
#include "KW_gui.h"
#include "KW_gui_internal.h"
#include "KW_eventwatcher.h"

#include "SDL_ttf.h"



KW_GUI * KW_Init(SDL_Renderer * renderer, SDL_Texture * tileset) {
  struct KW_GUI * gui = calloc(sizeof(*gui), 1);
  
  gui->tileset = tileset;
  gui->renderer = renderer;
  gui->rootwidget = AllocWidget();
  gui->rootwidget->gui = gui;
  gui->evqueuelock = SDL_CreateMutex();

  SDL_AddEventWatch(KW_EventWatcher, (void*)gui);
  
  return gui;
}



void KW_SetRender(KW_GUI * gui, SDL_Renderer * renderer) {
  gui->renderer = renderer;
}

void KW_SetTileset(KW_GUI * gui, SDL_Texture * tileset) {
  gui->tileset = tileset;
}


SDL_Renderer * KW_GetRenderer(KW_GUI * gui) {
  return gui->renderer;
}


void KW_SetRenderer(KW_GUI * gui, SDL_Renderer * renderer) {
  gui->renderer = renderer;
}


SDL_Texture * KW_GetTileset(KW_GUI * gui) {
  return gui->tileset;
}


void KW_Quit(KW_GUI * gui) {
  KW_DestroyWidget(gui->rootwidget, 1);
  free(gui);
}

void KW_SetFont(KW_GUI * gui, TTF_Font * font) {
  if (font == NULL) return;
  gui->font = font;
  return;
}

TTF_Font * KW_GetFont(KW_GUI * gui) {
  return gui->font;
}


KW_GUI * KW_GetGUI(KW_Widget * widget) {
  return KW_GetWidgetGUI(widget);
}

void KW_Paint(KW_GUI * gui) {
  KW_ProcessEvents(gui);
  KW_PaintWidget(gui->rootwidget);
}
