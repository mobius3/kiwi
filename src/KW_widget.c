#include "KW_widget_internal.h"
#include "KW_gui_internal.h"
#include "KW_widget.h"
#include <stdlib.h>

KW_Widget * AllocWidget() {
  KW_Widget * widget = calloc(1, sizeof(KW_Widget));
  return widget;
}


KW_Widget * KW_CreateWidget(KW_GUI * gui, KW_Widget * parent, KW_WidgetType type, const SDL_Rect * geometry, KW_WidgetPaintFunction widgetpaint, KW_WidgetDestroyFunction widgetdestroy, void * priv) {
  KW_Widget * widget = AllocWidget();
  widget->gui = gui;
  widget->paint = widgetpaint;
  widget->destroy = widgetdestroy;
  widget->type = type;
  /* set initial area as geometry */
  widget->composed = *geometry;
  KW_ReparentWidget(widget, parent);
  KW_SetWidgetGeometry(widget, geometry);
  KW_UnblockWidgetInputEvents(widget);
  widget->privdata = priv;
  widget->tileset = NULL;
  return widget;
}

KW_Widget * KW_GetWidgetParent(KW_Widget * widget) {
  return widget->parent == widget->gui->rootwidget ? NULL : widget->parent;
}

void KW_SetWidgetTileset(KW_Widget * widget, SDL_Texture * tileset) {
  widget->tileset = tileset;
}

SDL_Texture * KW_GetWidgetTileset(KW_Widget * widget) {
  return widget->tileset == NULL ? widget->gui->tileset : widget->tileset;
}

void * KW_GetWidgetData(KW_Widget * widget, KW_WidgetType type) {
  if (widget->type != type) return NULL;
  return widget->privdata;
}

SDL_Renderer * KW_GetWidgetRenderer(KW_Widget * widget) {
  return KW_GetRenderer(KW_GetGUI(widget));
}

void KW_GetWidgetAbsoluteGeometry(KW_Widget * widget, SDL_Rect * geometry) {
  SDL_Rect tmp;
  KW_GetWidgetGeometry(widget, geometry);
  for (widget = KW_GetWidgetParent(widget); widget != NULL; widget = KW_GetWidgetParent(widget)) {
    KW_GetWidgetGeometry(widget, &tmp);
    geometry->x += tmp.x;
    geometry->y += tmp.y;
  }
}


void KW_SetWidgetData(KW_Widget * widget, void * data) {
  widget->privdata = data;
}

void KW_BlockWidgetInputEvents(KW_Widget * widget) {
  widget->inputblocked = SDL_TRUE;
}

void KW_UnblockWidgetInputEvents(KW_Widget * widget) {
  widget->inputblocked = SDL_FALSE;
}


void Reparent(KW_Widget * widget, KW_Widget * newparent);

void FreeWidget(KW_Widget * widget, int freechildren) {
  KW_Widget * tofree;
  
  /* recursively delete children */
  if (freechildren) {
    while (widget->childrencount > 0) {
      tofree = widget->children[0];
      Reparent(tofree, NULL);
      FreeWidget(tofree, freechildren);
    }
  }
  
  free(widget->children);
  if (widget->destroy != NULL) {
    widget->destroy(widget);
  }
  free(widget);
}

void KW_SetClipChildrenWidgets(KW_Widget * widget, SDL_bool shouldclip) {
  widget->clipchildren = shouldclip;
}


/* recursively calculate (childs->parent->parent->...->root) all composed geometries.
 * 
 * They are relative to its parent, thus, if a child has a .x coordinate less than
 * 0, it is outside the current widget box and it must be recalculated. The current
 * widget parent must also be notified of this change if we happend to be outside parent box */
void CalculateComposedGeometry(KW_Widget * widget) {

  SDL_Rect box;
  int i = 0;
  short changed = 0;
  
#ifndef NDEBUG
  int i__;  
  static int indent = 0;
  indent++;
#endif

#ifndef NDEBUG
  for (i__ = 0; i__ < indent; i__++) printf(" ");
  printf("Recalculating composed geometry of %p: %d\n", (void*)widget, widget->type);
#endif
  if (widget->geometry.x != widget->composed.x ||
      widget->geometry.y != widget->composed.y ||
      widget->geometry.w != widget->composed.w ||
      widget->geometry.h != widget->composed.h)
  {
    changed++;
    widget->composed = widget->geometry;
#ifndef NDEBUG
    for (i__ = 0; i__ < indent; i__++) printf(" ");
    printf("Changed because of geometry\n");
#endif
  }
  for (i = 0; i < widget->childrencount; i++) {
    KW_GetWidgetComposedGeometry(widget->children[i], &box);
    if (box.x < 0) {
      widget->composed.x += box.x;
#ifndef NDEBUG      
      for (i__ = 0; i__ < indent; i__++) printf(" ");
      printf("Changed x!\n");
#endif
      changed++;
    }
    if (box.y < 0) {
      widget->composed.y += box.y;
#ifndef NDEBUG
      for (i__ = 0; i__ < indent; i__++) printf(" ");
      printf("Changed y!\n");
#endif
      changed++;
    }
    if (box.w + box.x > widget->composed.w) {
      widget->composed.w = box.w + box.x;
#ifndef NDEBUG
      for (i__ = 0; i__ < indent; i__++) printf(" ");
      printf("Changed w!\n");
#endif
      changed++;
    }
    if (box.h + box.y > widget->composed.h) {
      widget->composed.h = box.h + box.y;
#ifndef NDEBUG
      for (i__ = 0; i__ < indent; i__++) printf(" ");
      printf("Changed h!\n");
#endif
      changed++;
    }
  }
  
#ifndef NDEBUG  
  if (changed) {
    for (i__ = 0; i__ < indent; i__++) printf(" ");
    printf("new geom is %dx%d+%dx%d\n", widget->composed.x, widget->composed.y, widget->composed.w, widget->composed.h);
  } else {
    for (i__ = 0; i__ < indent; i__++) printf(" ");
    printf("nothing changed\n");
  }
#endif
  
  if (changed && widget->parent != NULL) {
#ifndef NDEBUG
    for (i__ = 0; i__ < indent; i__++) printf(" ");
    printf("Composed geometry did change!\n");
#endif
    /* see if our composite geometry change is relevant to parent */
    KW_GetWidgetComposedGeometry(widget->parent, &box);
    if ((    (widget->composed.x < box.x)
         || (widget->composed.y < box.y)
         || (widget->composed.w + widget->composed.x > box.w)
         || (widget->composed.h + widget->composed.y > box.h)
        )
        ||
        (widget->parent->parent == NULL) /* always calculate root geometry */
       )
    {
#ifndef NDEBUG
      for (i__ = 0; i__ < indent; i__++) printf(" ");
      printf("Gotta recalculate parent geometry!\n");
#endif
      CalculateComposedGeometry(widget->parent);
    }
  }
#ifndef NDEBUG
  indent--;
#endif
  /* TODO: callback for changed? */
}

void KW_GetWidgetComposedGeometry(KW_Widget * widget, SDL_Rect * composed) {
  *composed = widget->composed;
}


/* reparent version that allow us to parent to NULL */
void Reparent(KW_Widget * widget, KW_Widget * newparent) {
  int i = 0, j = -1;  
#ifndef NDEBUG
  printf("Reparenting %p: %d from %p to %p\n", (void*) widget, widget->type, (void*) widget->parent, (void*) newparent);
#endif
  /* gotta remove from previous parent */

  if (widget->parent != NULL) {
    KW_Widget * wp = widget->parent;
    
    /* iterate to find the position of widget */
    for (i = 0; i < wp->childrencount; i++) {
      if (wp->children[i] == widget) {
        j = i;
      }
      
      /* move everything in front of it */
      if (j >= 0 && i + 1 < wp->childrencount) {
        wp->children[i] = wp->children[i+1];
      }
    }
    
    /* realloc children array */
    wp->childrencount--;
    if (wp->childrencount <= 0) {
      free(wp->children);
      wp->children = NULL;
    }
    else
      wp->children = realloc(wp->children, wp->childrencount * sizeof(KW_Widget *));
    
    /* need to recalculate old parent area */
    CalculateComposedGeometry(widget);    
  }
  
  if (newparent != NULL) {
    newparent->childrencount++;
    newparent->children = realloc(newparent->children, newparent->childrencount * sizeof(KW_Widget *));
    newparent->children[newparent->childrencount-1] = widget;
    widget->parent = newparent;
    
    /* need to recalculate new parent area */
    CalculateComposedGeometry(newparent);
  }
}


/* public reparent that forces parenting to the root */
void KW_ReparentWidget(KW_Widget * widget, KW_Widget * newparent) {
  
  /* don't allow a null parent. not for users, at least. */
  if (newparent == NULL)
    newparent = widget->gui->rootwidget;
  
  /* don't allow widgets from different guis to interact */
  if (newparent->gui != widget->gui)
    return;
  
  if (newparent == widget->parent)
    return;
  
  Reparent(widget, newparent);
}

void KW_DestroyWidget(KW_Widget * widget, int freechildren) {
  /* Reparent every child of widget to widget->parent */
  if (!freechildren) {
    while (widget->childrencount > 0) {
      KW_ReparentWidget(widget->children[0], widget->parent);
    }
  }
  FreeWidget(widget, freechildren);
}

void KW_PaintWidget(KW_Widget * root) {
  int i = 0;
  
  /* paint the root, then paint its childrens */
  if (root->paint != NULL) {
    root->paint(root);
  }
  
  if (root->clipchildren) {
    SDL_RenderGetClipRect(KW_GetWidgetRenderer(root), &(root->oldcliprect));
    SDL_RenderSetClipRect(KW_GetWidgetRenderer(root), &(root->geometry));
  }
  for (i = 0; i < root->childrencount; i++) {
    KW_PaintWidget(root->children[i]);
  }
  if (root->clipchildren) {
    SDL_RenderSetClipRect(KW_GetWidgetRenderer(root), &(root->oldcliprect));
  }    
}

void KW_SetWidgetGeometry(KW_Widget * widget, const SDL_Rect * geometry) {
  if (widget->geometry.x != geometry->x ||
      widget->geometry.y != geometry->y ||
      widget->geometry.w != geometry->w ||
      widget->geometry.h != geometry->h)
  {
#ifndef NDEBUG
    printf("Set Geometry of %p: %d from %dx%d+%dx%d to %dx%d+%dx%d\n", (void*) widget, widget->type, widget->geometry.x, widget->geometry.y, widget->geometry.w, widget->geometry.h, geometry->x, geometry->y, geometry->w, geometry->h);
#endif
    widget->geometry = *geometry;
    CalculateComposedGeometry(widget);
  }
}

void KW_GetWidgetGeometry(KW_Widget * widget, SDL_Rect * geometry) {
  *geometry = widget->geometry;
}


KW_GUI * KW_GetWidgetGUI(KW_Widget * widget) {
  return widget->gui;
}

void KW_AddWidgetMouseOverHandler(KW_Widget * widget, KW_OnMouseOver handler) {
  AddWidgetHandler(widget, KW_ON_MOUSEOVER, (WidgetHandler) handler);
}

void KW_RemoveWidgetMouseOverHandler(KW_Widget * widget, KW_OnMouseOver handler) {
  RemoveWidgetHandler(widget, KW_ON_MOUSEOVER, (WidgetHandler) handler);
}

void KW_AddWidgetMouseLeaveHandler(KW_Widget * widget, KW_OnMouseLeave handler) {
  AddWidgetHandler(widget, KW_ON_MOUSELEAVE, (WidgetHandler) handler);
}

void KW_RemoveWidgetMouseLeaveHandler(KW_Widget * widget, KW_OnMouseLeave handler) {
  RemoveWidgetHandler(widget, KW_ON_MOUSELEAVE, (WidgetHandler) handler);
}

void KW_AddWidgetMouseDownHandler(KW_Widget * widget, KW_OnMouseDown handler) {
  AddWidgetHandler(widget, KW_ON_MOUSEDOWN, (WidgetHandler) handler);
}

void KW_RemoveWidgetMouseDownHandler(KW_Widget * widget, KW_OnMouseDown handler) {
  RemoveWidgetHandler(widget, KW_ON_MOUSEDOWN, (WidgetHandler) handler);
}

void KW_AddWidgetMouseUpHandler(KW_Widget * widget, KW_OnMouseDown handler) {
  AddWidgetHandler(widget, KW_ON_MOUSEUP, (WidgetHandler) handler);
}

void KW_RemoveWidgetMouseUpHandler(KW_Widget * widget, KW_OnMouseDown handler) {
  RemoveWidgetHandler(widget, KW_ON_MOUSEUP, (WidgetHandler) handler);
}

void KW_AddWidgetFocusGainHandler(KW_Widget * widget, KW_OnFocusGain handler) {
  AddWidgetHandler(widget, KW_ON_FOCUSGAIN, (WidgetHandler) handler);
}

void KW_RemoveWidgetFocusGainHandler(KW_Widget * widget, KW_OnFocusGain handler) {
  RemoveWidgetHandler(widget, KW_ON_FOCUSLOSE, (WidgetHandler) handler);
}

void KW_AddWidgetFocusLoseHandler(KW_Widget * widget, KW_OnFocusLose handler) {
  AddWidgetHandler(widget, KW_ON_FOCUSLOSE, (WidgetHandler) handler);
}

void KW_RemoveWidgetFocusLoseHandler(KW_Widget * widget, KW_OnFocusLose handler) {
  RemoveWidgetHandler(widget, KW_ON_FOCUSLOSE, (WidgetHandler) handler);
}

void KW_AddWidgetTextInputHandler(KW_Widget * widget, KW_OnTextInput handler) {
  AddWidgetHandler(widget, KW_ON_TEXTINPUT, (WidgetHandler) handler);
}

void KW_RemoveWidgetTextInputHandler(KW_Widget * widget, KW_OnTextInput handler) {
  RemoveWidgetHandler(widget, KW_ON_TEXTINPUT, (WidgetHandler) handler);
}

void KW_AddWidgetKeyDownHandler(KW_Widget * widget, KW_OnKeyDown handler) {
  AddWidgetHandler(widget, KW_ON_KEYDOWN, (WidgetHandler) handler);
}

void KW_RemoveWidgetKeyDownHandler(KW_Widget * widget, KW_OnKeyDown handler) {
  RemoveWidgetHandler(widget, KW_ON_KEYDOWN, (WidgetHandler) handler);
}

void KW_AddWidgetKeyUpHandler(KW_Widget * widget, KW_OnKeyUp handler) {
  AddWidgetHandler(widget, KW_ON_KEYUP, (WidgetHandler) handler);
}

void KW_RemoveWidgetKeyUpHandler(KW_Widget * widget, KW_OnKeyUp handler) {
  RemoveWidgetHandler(widget, KW_ON_KEYUP, (WidgetHandler) handler);
}

