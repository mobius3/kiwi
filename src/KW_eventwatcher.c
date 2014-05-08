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

void TextInputReady(KW_GUI * gui, const char * text) {
  int count, i;
  KW_OnTextInput * handlers;
  if (gui->currentfocus == NULL) return;
  count = gui->currentfocus->eventhandlers[KW_ON_TEXTINPUT].count;
  handlers = (KW_OnTextInput*) gui->currentfocus->eventhandlers[KW_ON_TEXTINPUT].handlers;
  for (i = 0; i < count; i++) {
    handlers[i](gui->currentfocus, text);
  }
}

void KeyUp(KW_GUI * gui, SDL_Keycode key, SDL_Scancode scan) {
  int count, i;
  KW_OnKeyUp * handlers;
  if (gui->currentfocus == NULL) return;
  count = gui->currentfocus->eventhandlers[KW_ON_KEYUP].count;
  handlers = (KW_OnKeyUp*) gui->currentfocus->eventhandlers[KW_ON_KEYUP].handlers;
  for (i = 0; i < count; i++) {
    handlers[i](gui->currentfocus, key, scan);
  }
}

void KeyDown(KW_GUI * gui, SDL_Keycode key, SDL_Scancode scan) {
  int count, i;
  KW_OnKeyDown * handlers;
  if (gui->currentfocus == NULL) return;
  count = gui->currentfocus->eventhandlers[KW_ON_KEYDOWN].count;
  handlers = (KW_OnKeyDown*) gui->currentfocus->eventhandlers[KW_ON_KEYDOWN].handlers;
  for (i = 0; i < count; i++) {
    handlers[i](gui->currentfocus, key, scan);
  }
}

/* to capture mouse movements, clicks, types, etc */
int KW_EventWatcher(void * data, SDL_Event * event) {
  KW_GUI * gui = (KW_GUI *) data;
  SDL_LockMutex(gui->evqueuelock);    
  gui->evqueue[(gui->evqueuesize)++] = *event;
  SDL_UnlockMutex(gui->evqueuelock);  
  return 0;
}

int KW_ProcessEvents(KW_GUI * gui) {
  int i = 0;
  SDL_LockMutex(gui->evqueuelock);  
  for (i = 0; i < gui->evqueuesize; i++) {
    SDL_Event * event = gui->evqueue + i;
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
        
      case SDL_TEXTINPUT:
        SDL_Log("Got a textready: %s\n", event->text.text);
        TextInputReady(gui, event->text.text);
        break;
        
      case SDL_TEXTEDITING:
        SDL_Log("Got a textediting: %d %d %d %s\n", event->edit.start, event->edit.length, event->edit.type, event->edit.text);
        break;
        
      case SDL_KEYDOWN:
        KeyDown(gui, event->key.keysym.sym, event->key.keysym.scancode);
        break;
      case SDL_KEYUP:
        KeyUp(gui, event->key.keysym.sym, event->key.keysym.scancode);
        break;      
      default:
        break;
    }
  }
  gui->evqueuesize = 0;
  SDL_UnlockMutex(gui->evqueuelock);
  
  return 0;
}
#undef NDEBUG
