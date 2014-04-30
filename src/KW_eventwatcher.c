#include "KW_eventwatcher.h"
#include "KW_gui_internal.h"
#include "KW_widget_internal.h"

KW_Widget * CalculateMouseOver(KW_Widget * widget, int x, int y) {
  /* calculate total area of this widget */
  
  
  /* walk down the children tree looking for a match */
  int i = 0;
  KW_Widget * test;
  SDL_Rect g;
  SDL_Rect offset;
  printf("Mouse over start\n");
  
  /* test on us first */
  KW_GetWidgetAbsoluteGeometry(widget, &offset);
  KW_GetWidgetComposedGeometry(widget, &g);
  g.x += offset.x;
  g.y += offset.y;
  if (!(x > g.x && x < g.x + g.w && y > g.y && y < g.y + g.h)) {
#ifndef NDEBUG
    printf(" Mouse is not over this geometry.\n");
#endif
    return NULL;
  }
#ifndef NDEBUG
  else {
    printf(" Mouse over %p: %d\n", (void*) widget, widget->type);
  }
#endif
  
  for (i = 0; i < widget->childrencount; i++) {
    /* select a children to test */
    test = widget->children[i];
    KW_GetWidgetComposedGeometry(test, &g);
    g.x += offset.x;
    g.y += offset.y;
    /* se if cursor is over this geometry */
    if (!test->inputblocked && (x > g.x && x < g.x + g.w && y > g.y && y < g.y + g.h)) {
      /* recalculate offset */
      offset.x += test->geometry.x;
      offset.y += test->geometry.y;
      widget = test;
#ifndef NDEBUG
      printf(" Mouse over %p: %d [%dx%d+%dx%d] (new offx=%d, offy=%d)\n", (void*) widget, widget->type, g.x, g.y, g.w, g.h, offset.x, offset.y);
#endif
      i = -1;
    }
  }
  
#ifndef NDEBUG
  printf("Top-most mouse over is %p: %d\n", (void*) widget, widget->type);
#endif
  return widget;
}

void MouseMoved(KW_GUI * gui, int mousex, int mousey) {
  KW_Widget * widget = CalculateMouseOver(gui->rootwidget, mousex, mousey);
  if (widget != NULL && widget->mouseover != NULL)
    widget->mouseover(widget);
}


/* to capture mouse movements, clicks, types, etc */
int KW_EventWatcher(void * data, SDL_Event * event) {
  KW_GUI * gui = (KW_GUI *) data;
  switch (event->type) {
    case SDL_MOUSEMOTION:
      MouseMoved(gui, event->motion.x, event->motion.y);
      break;
    default:
      break;
  }
  
  return 1;
}