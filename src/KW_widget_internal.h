#ifndef KW_WIDGET_INTERNAL
#define KW_WIDGET_INTERNAL

#include "KW_widget.h"

struct KW_GUI;

typedef enum KW_WidgetEventHandlerType {
  KW_ON_MOUSEOVER,
  KW_ON_MOUSELEAVE,
  KW_ON_MOUSEDOWN,
  KW_ON_MOUSEUP,
  KW_ON_FOCUSGAIN,
  KW_ON_FOCUSLOSE,
  KW_ON_TEXTINPUT,
  KW_ON_KEYDOWN,
  KW_ON_KEYUP,
  KW_ON_DRAGSTART,
  KW_ON_DRAGSTOP,
  KW_ON_DRAG,
  KW_ON_GEOMETRYCHANGED,
  KW_ON_CHILDRENCHANGE,
  KW_EVENTHANDLER_TOTAL
} KW_WidgetEventHandlerType;

typedef void (*WidgetHandler)(void);

void AddWidgetHandler(KW_Widget * widget, KW_WidgetEventHandlerType handlertype, WidgetHandler handler);
void RemoveWidgetHandler(KW_Widget * widget, KW_WidgetEventHandlerType handlertype, WidgetHandler handler);

struct KW_Widget {
  KW_WidgetType type;
  unsigned int childrencount;
  struct KW_GUI *     gui;
  SDL_Rect            absolute;
  SDL_Rect            geometry;
  SDL_Rect            composed;
  Uint32              hints;
  
  struct KW_Widget ** children;
  struct KW_Widget *  parent;

  
  SDL_Texture *       tilesettexture;
  SDL_Surface *       tilesetsurface;
  
  void                (*paint)(KW_Widget *);
  void                (*destroy)(KW_Widget *);
  
  struct {
    WidgetHandler *   handlers;
    unsigned int      count;
  } eventhandlers[KW_EVENTHANDLER_TOTAL];
  
  
  SDL_Rect            oldcliprect;
  SDL_bool            clipchildren;
  
  void *              userdata;
  void *              privdata;
};

KW_Widget * AllocWidget();

void FreeWidget(KW_Widget * widget, int freechildren);

#endif
