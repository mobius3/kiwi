#ifndef KW_WIDGET_INTERNAL
#define KW_WIDGET_INTERNAL

#include "KW_widget.h"

struct KW_GUI;

struct KW_Widget {
  int type;
  unsigned int childrencount;
  struct KW_Widget ** children;
  struct KW_Widget *  parent;
  struct KW_GUI *     gui;
  SDL_Rect            geometry;
  SDL_Rect            composed;
  SDL_bool            inputblocked;
  
  void                (*paint)(KW_Widget *);
  void                (*destroy)(KW_Widget *);
  
  KW_OnMouseOver *    mouseover;
  unsigned int        mouseovercount;
  
  KW_OnMouseLeave *   mouseleave;
  unsigned int        mouseleavecount;

  KW_OnMouseDown *    mousedown;
  unsigned int        mousedowncount;

  KW_OnMouseUp *      mouseup;
  unsigned int        mouseupcount;
  
  void *              privdata;
};

KW_Widget * AllocWidget();

void FreeWidget(KW_Widget * widget, int freechildren);

#endif