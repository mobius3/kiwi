#include "KW_eventwatcher.h"
#include "KW_gui_internal.h"
#include "KW_widget_internal.h"

KW_Widget * CalculateMouseOver(KW_Widget * widget, int x, int y) {
  /* calculate total area of this widget */
  
  
  /* walk down the children tree looking for a match */
  int i = 0;
  KW_Widget * test;
  SDL_Rect g = {0, 0, 0, 0};
  SDL_Rect offset = {0, 0, 0, 0};
  SDL_bool found = SDL_FALSE;
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
      found = SDL_TRUE;
#if !defined(NDEBUG) && defined(MOUSEOVER_DEBUG)
      printf(" Mouse over %p: %d [%dx%d+%dx%d] (new offx=%d, offy=%d)\n", (void*) widget, widget->type, g.x, g.y, g.w, g.h, offset.x, offset.y);
#endif
      i = -1;
    }
  }
  
#if !defined(NDEBUG) && defined(MOUSEOVER_DEBUG)
  if (found)
    printf("Top-most mouse over is %p: %d\n", (void*) widget, widget->type);
#endif
  return found ? widget : NULL;
}

void MouseMoved(KW_GUI * gui, int mousex, int mousey) {
  int i;
  KW_Widget * current = gui->currentmouseover;
  KW_Widget * widget = CalculateMouseOver(gui->rootwidget, mousex, mousey);
  if (widget == current) return;
  
  /* gotta notify the previous mouseover */
  if (current != NULL) {
    for (i = 0; i < current->mouseleavecount; i++) {
      current->mouseleave[i](current);
    }
  }

  /* warn the current mouseover */
  gui->currentmouseover = widget;
  if (widget != NULL && widget->mouseovercount > 0) {
    for (i = 0; i < widget->mouseovercount; i++) {
      widget->mouseover[i](widget);
    }
  }
}


void MousePressed(KW_GUI * gui, int mousex, int mousey, int button) {
  int i;
  KW_Widget * widget = gui->currentmouseover;
  if (widget != NULL && widget->mousedowncount > 0) {
    for (i = 0; i < widget->mousedowncount; i++) {
      widget->mousedown[i](widget, button);
    }
  }
}

void MouseReleased(KW_GUI * gui, int mousex, int mousey, int button) {
  int i;
  KW_Widget * widget = gui->currentmouseover;
  if (widget != NULL && widget->mouseupcount > 0) {
    for (i = 0; i < widget->mouseupcount; i++) {
      widget->mouseup[i](widget, button);
    }
  }
}

/* to capture mouse movements, clicks, types, etc */
int KW_EventWatcher(void * data, SDL_Event * event) {
  KW_GUI * gui = (KW_GUI *) data;
  switch (event->type) {
    case SDL_MOUSEMOTION:
      MouseMoved(gui, event->motion.x, event->motion.y);
      break;
    case SDL_MOUSEBUTTONDOWN:
      MousePressed(gui, event->button.x, event->button.y, event->button.button);
      break;
      
    case SDL_MOUSEBUTTONUP:
      MouseReleased(gui, event->button.x, event->button.y, event->button.button);
      break;
    default:
      break;
  }
  
  return 1;
}
#undef NDEBUG