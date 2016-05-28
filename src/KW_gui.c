#include "KW_widget_internal.h"
#include "KW_widget.h"
#include "KW_gui.h"
#include "KW_gui_internal.h"
#include "KW_eventwatcher.h"
#include "KW_renderdriver.h"

KW_GUI * KW_Init(KW_RenderDriver * renderer, KW_Surface * tileset) {
  struct KW_GUI * gui = calloc(sizeof(*gui), 1);
  gui->renderer = renderer;
  KW_SetTilesetSurface(gui, tileset);
  gui->rootwidget = AllocWidget();
  gui->rootwidget->gui = gui;
  gui->evqueuelock = SDL_CreateMutex();
  SDL_AddEventWatch(KW_EventWatcher, (void*)gui);
  
  return gui;
}



void KW_SetRender(KW_GUI * gui, KW_RenderDriver * renderer) {
  gui->renderer = renderer;
}

void KW_SetTilesetSurface(KW_GUI * gui, KW_Surface * tileset) {
  unsigned i = 0;
  KW_OnWidgetTilesetChange handler;
  KW_Widget * widget;
  gui->tilesettexture = KW_CreateTexture(gui->renderer, tileset);
  gui->tilesetsurface = tileset;
  for (i = 0; i < gui->eventhandlers[KW_GUI_ONTILESETCHANGED].count; i++) {
    widget = gui->eventhandlers[KW_GUI_ONTILESETCHANGED].handlers[i].priv;
    handler = (KW_OnWidgetTilesetChange) gui->eventhandlers[KW_GUI_ONTILESETCHANGED].handlers[i].handler;
    handler(widget);
  }
}


KW_RenderDriver * KW_GetRenderer(KW_GUI * gui) {
  return gui->renderer;
}


void KW_SetRenderer(KW_GUI * gui, KW_RenderDriver * renderer) {
  gui->renderer = renderer;
}


KW_Texture * KW_GetTilesetTexture(KW_GUI * gui) {
  return gui->tilesettexture;
}

KW_Texture * KW_GetTilesetSurface(KW_GUI * gui) {
  return gui->tilesetsurface;
}


void KW_Quit(KW_GUI * gui) {
  KW_DestroyWidget(gui->rootwidget, 1);
  free(gui);
}

void KW_SetFont(KW_GUI * gui, KW_Font * font) {
  unsigned i = 0;
  KW_OnGUIFontChanged handler;
  if (font == NULL) return;
  gui->font = font;

  for (i = 0; i < gui->eventhandlers[KW_GUI_ONFONTCHANGED].count; i++) {
    handler = (KW_OnGUIFontChanged) gui->eventhandlers[KW_GUI_ONFONTCHANGED].handlers[i].handler;
    handler(gui, gui->eventhandlers[KW_GUI_ONFONTCHANGED].handlers[i].priv, font);
  }
  return;
}

void KW_SetFontColor(KW_GUI * gui, KW_Color color) {
  unsigned i = 0;
  KW_OnGUIFontColorChanged handler;
  gui->fontcolor = color;
  for (i = 0; i < gui->eventhandlers[KW_GUI_ONFONTCOLORCHANGED].count; i++) {
    handler = (KW_OnGUIFontColorChanged) gui->eventhandlers[KW_GUI_ONFONTCOLORCHANGED].handlers[i].handler;
    handler(gui, gui->eventhandlers[KW_GUI_ONFONTCOLORCHANGED].handlers[i].priv, color);
  }
  return;
}

void KW_AddGUIFontChangedHandler(KW_GUI * gui, KW_OnGUIFontChanged handler, void * priv) {
  AddGUIHandler(gui, KW_GUI_ONFONTCHANGED, (GUIHandler) handler, priv);
}

void KW_RemoveGUIFontChangedHandler(KW_GUI * gui, KW_OnGUIFontChanged handler, void * priv) {
  RemoveGUItHandler(gui, KW_GUI_ONFONTCHANGED, (GUIHandler) handler, priv);
}

void KW_AddGUIFontColorChangedHandler(KW_GUI * gui, KW_OnGUIFontColorChanged handler, void * priv) {
  AddGUIHandler(gui, KW_GUI_ONFONTCOLORCHANGED, (GUIHandler) handler, priv);
}

void KW_RemoveGUIFontColorChangedHandler(KW_GUI * gui, KW_OnGUIFontColorChanged handler, void * priv) {
  RemoveGUItHandler(gui, KW_GUI_ONFONTCOLORCHANGED, (GUIHandler) handler, priv);
}

KW_Font * KW_GetFont(KW_GUI * gui) {
  return gui->font;
}

KW_Color KW_GetFontColor(KW_GUI * gui) {
  return gui->fontcolor;
}


KW_GUI * KW_GetGUI(const KW_Widget * widget) {
  return KW_GetWidgetGUI(widget);
}

void KW_Paint(KW_GUI * gui) {
  KW_PaintWidget(gui->rootwidget);
}

/* generic handler list functions */

void AddGUIHandler(KW_GUI * gui, KW_GUIEventHandlerType handlertype, GUIHandler handler, void * priv) {
  /* don't add multiple mouse over handlers */
  unsigned int * count = &(gui->eventhandlers[handlertype].count);
  
  /* check if handler is there */
  unsigned i;
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
  unsigned i; int j = -1;
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
