#include <time.h>
#include <stdlib.h>

#include "kiwi/core/widget.h"
#include "kiwi/core/gui.h"
#include "kiwi/core/old-renderdriver.h"

#include "widget-internal.h"
#include "gui-internal.h"

KW_GUI * KW_CreateGUI(KW_OldRenderDriver * renderer, KW_Surface * tileset) {
  struct KW_GUI * gui = calloc(sizeof(*gui), 1);
  gui->renderer = renderer;
  KW_SetGUITilesetSurface(gui, tileset);
  gui->rootwidget = AllocWidget();
  gui->rootwidget->gui = gui;
  gui->evqueuelock = SDL_CreateMutex();
  gui->handleevents = SDL_TRUE;
  srand((unsigned) time(NULL));
  
  return gui;
}

void KW_SetGUIRenderer(KW_GUI * gui, KW_OldRenderDriver * renderer) {
  gui->renderer = renderer;
}

void KW_SetGUITilesetSurface(KW_GUI * gui, KW_Surface * tileset) {
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


KW_OldRenderDriver * KW_GetGUIRenderer(KW_GUI * gui) {
  return gui->renderer;
}

KW_Texture * KW_GetGUITilesetTexture(KW_GUI * gui) {
  return gui->tilesettexture;
}

KW_Surface * KW_GetGUITilesetSurface(KW_GUI * gui) {
  return gui->tilesetsurface;
}


void KW_DestroyGUI(KW_GUI * gui) {
  KW_DestroyWidget(gui->rootwidget, 1);
  KW_ReleaseFont(gui->renderer, gui->defaultfont);
  KW_ReleaseTexture(gui->renderer, gui->tilesettexture);
  free(gui);
}

void KW_SetGUIFont(KW_GUI * gui, KW_Font * font) {
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

void KW_SetGUITextColor(KW_GUI * gui, KW_Color color) {
  unsigned i = 0;
  KW_OnGUITextColorChanged handler;
  gui->textcolor = color;
  for (i = 0; i < gui->eventhandlers[KW_GUI_ONTEXTCOLORCHANGED].count; i++) {
    handler = (KW_OnGUITextColorChanged) gui->eventhandlers[KW_GUI_ONTEXTCOLORCHANGED].handlers[i].handler;
    handler(gui, gui->eventhandlers[KW_GUI_ONTEXTCOLORCHANGED].handlers[i].priv, color);
  }
}

void KW_AddGUIFontChangedHandler(KW_GUI * gui, KW_OnGUIFontChanged handler, void * priv) {
  AddGUIHandler(gui, KW_GUI_ONFONTCHANGED, (GUIHandler) handler, priv);
}

void KW_RemoveGUIFontChangedHandler(KW_GUI * gui, KW_OnGUIFontChanged handler, void * priv) {
  RemoveGUIHandler(gui, KW_GUI_ONFONTCHANGED, (GUIHandler) handler, priv);
}

void KW_AddGUITextColorChangedHandler(KW_GUI * gui, KW_OnGUITextColorChanged handler, void * priv) {
  AddGUIHandler(gui, KW_GUI_ONTEXTCOLORCHANGED, (GUIHandler) handler, priv);
}

void KW_RemoveGUITextColorChangedHandler(KW_GUI * gui, KW_OnGUITextColorChanged handler, void * priv) {
  RemoveGUIHandler(gui, KW_GUI_ONTEXTCOLORCHANGED, (GUIHandler) handler, priv);
}

KW_Font * KW_GetGUIFont(KW_GUI * gui) {
  return gui->font ? gui->font : gui->defaultfont;
}

KW_Color KW_GetGUITextColor(KW_GUI * gui) {
  return gui->textcolor;
}


KW_GUI * KW_GetGUI(const KW_Widget * widget) {
  return KW_GetWidgetGUI(widget);
}

void KW_PaintGUI(KW_GUI * gui) {
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

void RemoveGUIHandler(KW_GUI * gui, KW_GUIEventHandlerType handlertype, GUIHandler handler, void * priv) {
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

void KW_HideGUI(KW_GUI * gui) {
  SDL_LockMutex(gui->evqueuelock);
  gui->handleevents = SDL_FALSE;
  /* make sure the queue is emptied */
  gui->evqueuesize = 0;
  SDL_UnlockMutex(gui->evqueuelock);
}

void KW_ShowGUI(KW_GUI * gui) {
  SDL_LockMutex(gui->evqueuelock);
  gui->handleevents = SDL_TRUE;
  /* make sure the queue is emptied */
  gui->evqueuesize = 0;
  SDL_UnlockMutex(gui->evqueuelock);
}

void KW_PostGUIInputEvent(KW_GUI * gui, KW_InputEvent const * event) {
  if(!gui->handleevents) return;
  gui->inputEventQueue[(gui->evqueuesize)++] = *event;
}


void KW_ProcessGUIEvents(KW_GUI * gui) {
  int i = 0;
  gui->cursorwasdown = SDL_FALSE;
  for (i = 0; i < gui->evqueuesize; i++) {
    KW_InputEvent * event = gui->inputEventQueue + i;
    switch (event->type) {
      case KW_INPUT_CURSOR_MOTION:
        MouseMoved(gui, event->cursorMotion.x, event->cursorMotion.y, event->cursorMotion.xMotion, event->cursorMotion.yMotion);
        break;
      case KW_INPUT_CURSOR_BUTTON_DOWN:
        MousePressed(gui, event->cursorButtonDown.x, event->cursorButtonDown.y, event->cursorButtonDown.button);
        break;

      case KW_INPUT_CURSOR_BUTTON_UP:
        MouseReleased(gui, event->cursorButtonUp.x, event->cursorButtonUp.y, event->cursorButtonUp.button);
        break;

      case KW_INPUT_TEXT:
        TextInputReady(gui, event->text.text);
        break;

      case KW_INPUT_KEY_DOWN:
        KeyDown(gui, event->keyUp.key);
        break;
      case KW_INPUT_KEY_UP:
        KeyUp(gui, event->keyDown.key);
        break;
      default:
        break;
    }
  }
  gui->evqueuesize = 0;
  SDL_UnlockMutex(gui->evqueuelock);
}
