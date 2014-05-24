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
  KW_EVENTHANDLER_TOTAL
} KW_WidgetEventHandlerType;

typedef void (*WidgetHandler)(void);

void AddWidgetHandler(KW_Widget * widget, KW_WidgetEventHandlerType handlertype, WidgetHandler handler);
void RemoveWidgetHandler(KW_Widget * widget, KW_WidgetEventHandlerType handlertype, WidgetHandler handler);

struct KW_Widget {
  int type;
  unsigned int childrencount;
  struct KW_Widget ** children;
  struct KW_Widget *  parent;
  struct KW_GUI *     gui;
  SDL_Rect            geometry;
  SDL_Rect            composed;
  SDL_bool            inputblocked;
  
  SDL_Texture *       tilesettexture;
  SDL_Surface *       tilesetsurface;
  
  void                (*paint)(KW_Widget *);
  void                (*destroy)(KW_Widget *);
  
  
  struct {
    WidgetHandler *   handlers;
    unsigned int      count;
  } eventhandlers[KW_EVENTHANDLER_TOTAL];
  
  KW_OnMouseOver *    mouseover;
  unsigned int        mouseovercount;
  
  KW_OnMouseLeave *   mouseleave;
  unsigned int        mouseleavecount;
  
  KW_OnFocusGain *    focusgain;
  unsigned int        focusgaincount;
  
  KW_OnFocusLose *    focuslose;
  unsigned int        focuslosecount;

  KW_OnMouseDown *    mousedown;
  unsigned int        mousedowncount;

  KW_OnMouseUp *      mouseup;
  unsigned int        mouseupcount;
  
  SDL_Rect            absolute;
  SDL_Rect            oldcliprect;
  SDL_bool            clipchildren;
  
  void *              privdata;
};

KW_Widget * AllocWidget();

void FreeWidget(KW_Widget * widget, int freechildren);

#endif
