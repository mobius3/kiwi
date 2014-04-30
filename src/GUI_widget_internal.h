#ifndef GUI_WIDGET_INTERNAL
#define GUI_WIDGET_INTERNAL

#include "GUI_widget.h"

struct GUI_GUI;

struct GUI_Widget {
  int type;
  unsigned int childrencount;
  struct GUI_Widget ** children;
  struct GUI_Widget * parent;
  struct GUI_GUI * gui;
  SDL_Rect geometry;
  SDL_Rect composed;
  
  void (*paint)(GUI_Widget *);
  void (*destroy)(GUI_Widget *);
  
  void (*mouseover)(GUI_Widget *);
  void (*mousedown)(GUI_Widget *, int button);
  void (*mouseup)(GUI_Widget *, int button);
  
  void * privdata;
};

GUI_Widget * AllocWidget();

void FreeWidget(GUI_Widget * widget, int freechildren);

#endif