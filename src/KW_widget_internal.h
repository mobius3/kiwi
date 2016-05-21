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
  KW_ON_TILESETCHANGE,
  KW_EVENTHANDLER_TOTAL
} KW_WidgetEventHandlerType;

typedef void (*WidgetHandler)(void);

void AddWidgetHandler(KW_Widget * widget, KW_WidgetEventHandlerType handlertype, WidgetHandler handler);
void RemoveWidgetHandler(KW_Widget * widget, KW_WidgetEventHandlerType handlertype, WidgetHandler handler);

struct KW_Widget {
  KW_WidgetType type;
  unsigned int childrencount;
  struct KW_GUI *     gui;
  KW_Rect             absolute;
  KW_Rect             geometry;
  KW_Rect             composed;
  unsigned int        hints;
  
  struct KW_Widget ** children;
  struct KW_Widget *  parent;

  
  KW_Texture *       tilesettexture;
  KW_Texture *       tilesetsurface;
  
  void                (*paint)(KW_Widget *);
  void                (*destroy)(KW_Widget *);
  
  struct {
    WidgetHandler *   handlers;
    unsigned int      count;
  } eventhandlers[KW_EVENTHANDLER_TOTAL];
  
  
  KW_Rect            oldcliprect;
  KW_bool            clipchildren;
  
  void *              userdata;
  void *              privdata;
};

KW_Widget * AllocWidget();

void FreeWidget(KW_Widget * widget, int freechildren);

#endif
