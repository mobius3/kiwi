#include "KW_widget_internal.h"
#include "KW_widget.h"
#include "KW_gui.h"
#include "KW_gui_internal.h"
#include "KW_eventwatcher.h"

#include "SDL_ttf.h"



KW_GUI * KW_Init(SDL_Renderer * renderer, SDL_Surface * tileset) {
  struct KW_GUI * gui = calloc(sizeof(*gui), 1);
  gui->renderer = renderer;
  KW_SetTilesetSurface(gui, tileset);
  gui->rootwidget = AllocWidget();
  gui->rootwidget->gui = gui;
  gui->evqueuelock = SDL_CreateMutex();

  SDL_AddEventWatch(KW_EventWatcher, (void*)gui);
  
  return gui;
}



void KW_SetRender(KW_GUI * gui, SDL_Renderer * renderer) {
  gui->renderer = renderer;
}

void KW_SetTilesetSurface(KW_GUI * gui, SDL_Surface * tileset) {
  gui->tilesetsurface = tileset;
  gui->tilesettexture = SDL_CreateTextureFromSurface(gui->renderer, tileset);
}


SDL_Renderer * KW_GetRenderer(KW_GUI * gui) {
  return gui->renderer;
}


void KW_SetRenderer(KW_GUI * gui, SDL_Renderer * renderer) {
  gui->renderer = renderer;
}


SDL_Texture * KW_GetTilesetTexture(KW_GUI * gui) {
  return gui->tilesettexture;
}

SDL_Surface * KW_GetTilesetSurface(KW_GUI * gui) {
  return gui->tilesetsurface;
}


void KW_Quit(KW_GUI * gui) {
  KW_DestroyWidget(gui->rootwidget, 1);
  free(gui);
}

void KW_SetFont(KW_GUI * gui, TTF_Font * font) {
  int i = 0;
  KW_OnGUIFontChanged handler;
  if (font == NULL) return;
  gui->font = font;

  for (i = 0; i < gui->eventhandlers[KW_GUI_ONFONTCHANGED].count; i++) {
    handler = (KW_OnGUIFontChanged) gui->eventhandlers[KW_GUI_ONFONTCHANGED].handlers[i].handler;
    handler(gui, gui->eventhandlers[KW_GUI_ONFONTCHANGED].handlers[i].priv, font);
  }
  return;
}

void KW_AddGUIFontChangedHandler(KW_GUI * gui, KW_OnGUIFontChanged handler, void * priv) {
  AddGUIHandler(gui, KW_GUI_ONFONTCHANGED, (GUIHandler) handler, priv);
}

void KW_RemoveGUIFontChangedHandler(KW_GUI * gui, KW_OnGUIFontChanged handler, void * priv) {
  RemoveGUItHandler(gui, KW_GUI_ONFONTCHANGED, (GUIHandler) handler, priv);
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

/* generic handler list functions */

void AddGUIHandler(KW_GUI * gui, KW_GUIEventHandlerType handlertype, GUIHandler handler, void * priv) {
  /* don't add multiple mouse over handlers */
  unsigned int * count = &(gui->eventhandlers[handlertype].count);
  
  /* check if handler is there */
  int i;
  for (i = 0; i < *count; ++i) {
    if (gui->eventhandlers[handlertype].handlers[i].handler == handler && gui->eventhandlers[handlertype].handlers[i].priv == priv)
      return;
  }
  
  /* add the handler then */
  (*count)++;
  gui->eventhandlers[handlertype].handlers = realloc(gui->eventhandlers[handlertype].handlers, sizeof(struct KW_GUICallback) * (*count));
  gui->eventhandlers[handlertype].handlers[(*count) - 1].handler = handler;  
  gui->eventhandlers[handlertype].handlers[(*count) - 1].priv = priv;
}

void RemoveGUItHandler(KW_GUI * gui, KW_GUIEventHandlerType handlertype, GUIHandler handler, void * priv) {
  int i, j;
  unsigned int * count = &(gui->eventhandlers[handlertype].count);
  
  /* iterate to find the position of gui */
  for (i = 0; i < *count; i++) {
    if (gui->eventhandlers[handlertype].handlers[i].handler == handler) {
      if (gui->eventhandlers[handlertype].handlers[i].priv == priv)
        j = i;
    }
    
    /* move everything in front of it */
    if (j >= 0 && i + 1 < *count) {
      gui->eventhandlers[handlertype].handlers[i] = gui->eventhandlers[handlertype].handlers[i+1];
    }
  }
  
  /* realloc children array */
  (*count)--;
  if (*count <= 0) {
    free(gui->eventhandlers[handlertype].handlers);
    gui->eventhandlers[handlertype].handlers = NULL;
  }
  else
    gui->eventhandlers[handlertype].handlers = realloc(gui->eventhandlers[handlertype].handlers, *count * sizeof(struct KW_GUICallback));  
}
