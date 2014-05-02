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
  KW_EVENTHANDLER_TOTAL
} KW_WidgetEventHandlerType;

void AddHandler(KW_Widget * widget, KW_WidgetEventHandlerType handlertype, void * handler);
void RemoveHandler(KW_Widget * widget, KW_WidgetEventHandlerType handlertype, void * handler);

struct KW_Widget {
  int type;
  unsigned int childrencount;
  struct KW_Widget ** children;
  struct KW_Widget *  parent;
  struct KW_GUI *     gui;
  SDL_Rect            geometry;
  SDL_Rect            composed;
  SDL_bool            inputblocked;
  
  SDL_Texture *       tileset;
  
  void                (*paint)(KW_Widget *);
  void                (*destroy)(KW_Widget *);
  
  
  struct {
    void **           handlers;
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
  
  void *              privdata;
};

KW_Widget * AllocWidget();

void FreeWidget(KW_Widget * widget, int freechildren);

#endif