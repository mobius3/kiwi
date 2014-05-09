#ifndef KW_KW_INTERNAL_H
#define KW_KW_INTERNAL_H

#include "KW_widget.h"
#include "SDL_ttf.h"

typedef enum KW_GUIEventHandlerType {
  KW_GUI_ONFONTCHANGED,
  KW_GUIEVENTHANDLER_TOTAL
} KW_GUIEventHandlerType;

struct KW_GUI {
  SDL_Texture * tileset;
  SDL_Renderer * renderer;
  KW_Widget * rootwidget; /* the parent to all parent-less widgets :) */
  TTF_Font * font;
  
  KW_Widget * currentmouseover; /* the current widget that has mouse over */
  KW_Widget * currentfocus; /* which widget was the last focused/clicked */
  
  SDL_Event evqueue[1024];
  int evqueuesize;
  SDL_mutex * evqueuelock;
  
  struct {
    struct KW_GUICallback {
      void * handler;
      void * priv;
    } * handlers;
    unsigned int      count;
  } eventhandlers[KW_GUIEVENTHANDLER_TOTAL];  
};

typedef struct KW_GUI KW_GUI;

void AddGUIHandler(KW_GUI * gui, KW_GUIEventHandlerType handlertype, void * handler, void * priv);
void RemoveGUItHandler(KW_GUI * gui, KW_GUIEventHandlerType handlertype, void * handler, void * priv);





#endif
