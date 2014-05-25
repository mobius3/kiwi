#include "KW_eventwatcher.h"
#include "KW_gui_internal.h"
#include "KW_widget_internal.h"

KW_Widget * CalculateMouseOver(KW_Widget * widget, int x, int y) {
  int i;
  KW_Widget * found = NULL;
  SDL_Rect g = widget->composed;
  if (widget->parent) {
    g.x += widget->parent->absolute.x;
    g.y += widget->parent->absolute.y;
  }
  /* mouseover is a input event, avoid calculating it */
  if (widget->inputblocked) { return NULL; }
  
  /* if not in composed geometry, nothing to see here then. rootwidget always borked. */
  if (!(x > g.x && x < g.x + g.w && y > g.y && y < g.y + g.h) && widget != widget->gui->rootwidget) { 
    return NULL;
  }
  
  for (i = widget->childrencount-1; i >= 0 && found == NULL; i--)
    found = CalculateMouseOver(widget->children[i], x, y);
  
  /* children don't have it. Maybe its this widget then? */
  if (found == NULL) {
    g = widget->absolute;
    /* use our own absolute to test, then */
    if (x > g.x && x < g.x + g.w && y > g.y && y < g.y + g.h)
      found = widget;
  }
  return found; 
}

void MouseMoved(KW_GUI * gui, int mousex, int mousey, int xrel, int yrel) {
  int i, count;
  KW_OnMouseOver * overhandlers;
  KW_OnMouseLeave * leavehandlers;
  KW_OnDragStart * dragstarthandlers;
  KW_OnDrag * draghandlers;
  KW_Widget * current = gui->currentmouseover;
  KW_Widget * widget = NULL;
  
  /* first check if we are in drag mode */
  if (gui->cursordown == SDL_TRUE) {
    /* check if drag is starting */
    if (gui->currentdrag == NULL) {
      /* drag is starting on the current mouse over */
      if (current != NULL) {
        /* report dragstart */
        count = current->eventhandlers[KW_ON_DRAGSTART].count;
        dragstarthandlers = (KW_OnDragStart *) current->eventhandlers[KW_ON_DRAGSTART].handlers;
        for (i = 0; i < count; i++) {
          dragstarthandlers[i](current, mousex, mousey);
        }
        /* sets the currend drag widget */
        gui->currentdrag = current;
      }
    } else {
      /* drag HAS started already. Update widh drag positions */
        count = current->eventhandlers[KW_ON_DRAG].count;
        draghandlers = (KW_OnDrag *) current->eventhandlers[KW_ON_DRAG].handlers;
        for (i = 0; i < count; i++) {
          draghandlers[i](current, mousex, mousey, xrel, yrel);
        }
    }
    
    /* no mouse movement events are calculated while dragging */
    return;
  }
  
  widget = CalculateMouseOver(gui->rootwidget, mousex, mousey);
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
  gui->cursordown = SDL_TRUE;
}

void MouseReleased(KW_GUI * gui, int mousex, int mousey, int button) {
  int i, count;
  KW_OnMouseUp * upandlers;
  KW_OnFocusGain * gainhandlers;
  KW_OnFocusLose * losehandlers;
  KW_OnDragStop * dragstophandlers;
  KW_Widget * widget = gui->currentmouseover;
  
  gui->cursordown = SDL_FALSE;
  
  /* check if was under drag */
  if (gui->currentdrag != NULL) {
    count = widget->eventhandlers[KW_ON_DRAGSTOP].count;
    dragstophandlers = (KW_OnDragStop *) widget->eventhandlers[KW_ON_DRAGSTOP].handlers;
    for (i = 0; i < count; i++) {
      dragstophandlers[i](widget, mousex, mousey);
    }
    gui->currentdrag = NULL;

    /* force a mouse move to calculate mouseover again. */
    MouseMoved(gui, mousex, mousey, 0, 0);
  }
  
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
        MouseMoved(gui, event->motion.x, event->motion.y, event->motion.xrel, event->motion.yrel);
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
