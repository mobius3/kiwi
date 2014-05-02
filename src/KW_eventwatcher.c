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
  int i, count;
  KW_OnMouseOver * overhandlers;
  KW_OnMouseLeave * leavehandlers;
  KW_Widget * current = gui->currentmouseover;
  KW_Widget * widget = CalculateMouseOver(gui->rootwidget, mousex, mousey);
  if (widget == current) return;
  
  /* gotta notify the previous mouseover */
  if (current != NULL) {
    count = current->eventhandlers[KW_ON_MOUSELEAVE].count;
    leavehandlers = (KW_OnMouseLeave *) current->eventhandlers[KW_ON_MOUSELEAVE].handlers;
    for (i = 0; i < count; i++) {
      leavehandlers[i](current);
    }
  }

  /* warn the current mouseover */
  gui->currentmouseover = widget;
  if (widget != NULL) {
    count = widget->eventhandlers[KW_ON_MOUSEOVER].count;
    overhandlers = (KW_OnMouseOver *) widget->eventhandlers[KW_ON_MOUSEOVER].handlers;
    for (i = 0; i < count; i++) {
      overhandlers[i](widget);
    }
  }
}


void MousePressed(KW_GUI * gui, int mousex, int mousey, int button) {
  int i, count;
  KW_OnMouseDown * handlers;
  KW_Widget * widget = gui->currentmouseover;
  if (widget != NULL) {
    count = widget->eventhandlers[KW_ON_MOUSEDOWN].count;
    handlers = (KW_OnMouseDown *) widget->eventhandlers[KW_ON_MOUSEDOWN].handlers;
    for (i = 0; i < count; i++) {
      handlers[i](widget, button);
    }
  }
}

void MouseReleased(KW_GUI * gui, int mousex, int mousey, int button) {
  int i, count;
  KW_OnMouseUp * upandlers;
  KW_OnFocusGain * gainhandlers;
  KW_OnFocusLose * losehandlers;
  KW_Widget * widget = gui->currentmouseover;
  if (widget != NULL) {
    count = widget->eventhandlers[KW_ON_MOUSEUP].count;
    upandlers = (KW_OnMouseUp *) widget->eventhandlers[KW_ON_MOUSEUP].handlers;
    for (i = 0; i < count; i++) {
      upandlers[i](widget, button);
    }
  }

  
  if (widget != NULL && widget != gui->currentfocus) {
    /* warn that its losing focus */
    if (gui->currentfocus != NULL) {
      count = gui->currentfocus->eventhandlers[KW_ON_FOCUSLOSE].count;
      losehandlers = (KW_OnFocusLose *) gui->currentfocus->eventhandlers[KW_ON_FOCUSLOSE].handlers;    
      for (i = 0; i < count; i++) {
        losehandlers[i](gui->currentfocus);
      }
    }
    
    /* watn that its gaining focus */
    count = widget->eventhandlers[KW_ON_FOCUSGAIN].count;
    gainhandlers = (KW_OnFocusGain *) widget->eventhandlers[KW_ON_FOCUSGAIN].handlers;        
    for (i = 0; i < count; i++) {
      gainhandlers[i](widget);
    }
    gui->currentfocus = widget;
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