#include "GUI_widget_internal.h"
#include "GUI_gui_internal.h"
#include "GUI_widget.h"
#include <stdlib.h>

GUI_Widget * AllocWidget() {
  GUI_Widget * widget = calloc(1, sizeof(GUI_Widget));
  return widget;
}


GUI_Widget * GUI_CreateWidget(GUI_GUI * gui, GUI_Widget * parent, GUI_WidgetType type, const SDL_Rect * geometry, GUI_WidgetPaintFunction widgetpaint, GUI_WidgetDestroyFunction widgetdestroy, void * priv) {
  GUI_Widget * widget = AllocWidget();
  widget->gui = gui;
  widget->paint = widgetpaint;
  widget->destroy = widgetdestroy;
  widget->type = type;
  /* set initial area as geometry */
  widget->composed = *geometry;
  GUI_ReparentWidget(widget, parent);
  GUI_SetWidgetGeometry(widget, geometry);
  widget->privdata = priv;
  
  return widget;
}

GUI_Widget * GUI_GetWidgetParent(GUI_Widget * widget) {
  return widget->parent == widget->gui->rootwidget ? NULL : widget->parent;
}

void * GUI_GetWidgetData(GUI_Widget * widget) {
  return widget->privdata;
}

SDL_Renderer * GUI_GetWidgetRenderer(GUI_Widget * widget) {
  return GUI_GetRenderer(GUI_GetGUI(widget));
}

void GUI_GetWidgetAbsoluteGeometry(GUI_Widget * widget, SDL_Rect * geometry) {
  SDL_Rect tmp;
  GUI_GetWidgetGeometry(widget, geometry);
  for (widget = GUI_GetWidgetParent(widget); widget != NULL; widget = GUI_GetWidgetParent(widget)) {
    GUI_GetWidgetGeometry(widget, &tmp);
    geometry->x += tmp.x;
    geometry->y += tmp.y;
  }
}


void GUI_SetWidgetData(GUI_Widget * widget, void * data) {
  widget->privdata = data;
}


void Reparent(GUI_Widget * widget, GUI_Widget * newparent);

void FreeWidget(GUI_Widget * widget, int freechildren) {
  GUI_Widget * tofree;
  
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

/* recursively calculate (childs->parent->parent->...->root) all composed geometries.
 * 
 * They are relative to its parent, thus, if a child has a .x coordinate less than
 * 0, it is outside the current widget box and it must be recalculated. The current
 * widget parent must also be notified of this change if we happend to be outside parent box */
void CalculateComposedGeometry(GUI_Widget * widget) {
  
#ifndef NDEBUG
  static int indent = 0;
  indent++;
  int i__;
#endif
  
  SDL_Rect box;
  int i = 0;
  short changed = 0;
#ifndef NDEBUG
  for (i = 0; i < indent; i++) printf(" ");
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
    GUI_GetWidgetComposedGeometry(widget->children[i], &box);
    if (box.x < 0) {
      widget->composed.x = box.x;
#ifndef NDEBUG      
      for (i__ = 0; i__ < indent; i__++) printf(" ");
      printf("Changed x!\n");
#endif
      changed++;
    }
    if (box.y < 0) {
      widget->composed.y = box.y;
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
    GUI_GetWidgetComposedGeometry(widget->parent, &box);
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

void GUI_GetWidgetComposedGeometry(GUI_Widget * widget, SDL_Rect * composed) {
  *composed = widget->composed;
}


/* reparent version that allow us to parent to NULL */
void Reparent(GUI_Widget * widget, GUI_Widget * newparent) {
#ifndef NDEBUG
  printf("Reparenting %p: %d from %p to %p\n", (void*) widget, widget->type, (void*) widget->parent, (void*) newparent);
#endif
  /* gotta remove from previous parent */
  int i = 0, j = -1;
  if (widget->parent != NULL) {
    GUI_Widget * wp = widget->parent;
    
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
      wp->children = realloc(wp->children, wp->childrencount * sizeof(GUI_Widget *));
    
    /* need to recalculate old parent area */
    CalculateComposedGeometry(widget);    
  }
  
  if (newparent != NULL) {
    newparent->childrencount++;
    newparent->children = realloc(newparent->children, newparent->childrencount * sizeof(GUI_Widget *));
    newparent->children[newparent->childrencount-1] = widget;
    widget->parent = newparent;
    
    /* need to recalculate new parent area */
    CalculateComposedGeometry(newparent);
  }
  

}


/* public reparent that forces parenting to the root */
void GUI_ReparentWidget(GUI_Widget * widget, GUI_Widget * newparent) {
  
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

void GUI_DestroyWidget(GUI_Widget * widget, int freechildren) {
  /* Reparent every child of widget to widget->parent */
  if (!freechildren) {
    while (widget->childrencount > 0) {
      GUI_ReparentWidget(widget->children[0], widget->parent);
    }
  }
  FreeWidget(widget, freechildren);
}

void GUI_PaintWidget(GUI_Widget * root) {
  int i = 0;
  
  /* paint the root, then paint its childrens */
  if (root->paint != NULL) {
    root->paint(root);
  }
  for (i = 0; i < root->childrencount; i++) {
    GUI_PaintWidget(root->children[i]);
  }
}

void GUI_SetWidgetGeometry(GUI_Widget * widget, const SDL_Rect * geometry) {
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

void GUI_GetWidgetGeometry(GUI_Widget * widget, SDL_Rect * geometry) {
  *geometry = widget->geometry;
}


GUI_GUI * GUI_GetWidgetGUI(GUI_Widget * widget) {
  return widget->gui;
}
