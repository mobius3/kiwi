#include "KW_widget_internal.h"
#include "KW_gui_internal.h"
#include "KW_widget.h"
#include <stdlib.h>
#include <unistd.h>

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
  widget->tilesettexture = NULL;
  widget->tilesetsurface = NULL;
  return widget;
}

KW_Widget * KW_GetWidgetParent(KW_Widget * widget) {
  return widget->parent == widget->gui->rootwidget ? NULL : widget->parent;
}

KW_Widget * const * KW_GetWidgetChildren(KW_Widget * widget, unsigned int * count) {
  *count = widget->childrencount;
  return widget->children;
}


void KW_SetWidgetTilesetSurface(KW_Widget * widget, SDL_Surface * tileset) {
  widget->tilesetsurface = tileset;
  widget->tilesettexture = SDL_CreateTextureFromSurface(KW_GetWidgetRenderer(widget), tileset);
}

SDL_Texture * KW_GetWidgetTilesetTexture(KW_Widget * widget) {
  return widget->tilesettexture == NULL ? widget->gui->tilesettexture : widget->tilesettexture;
}

SDL_Surface * KW_GetWidgetTilesetSurface(KW_Widget * widget) {
  return widget->tilesetsurface == NULL ? widget->gui->tilesetsurface : widget->tilesetsurface;
}

void * KW_GetWidgetData(KW_Widget * widget, KW_WidgetType type) {
  if (widget->type != type) return NULL;
  return widget->privdata;
}

SDL_Renderer * KW_GetWidgetRenderer(KW_Widget * widget) {
  return KW_GetRenderer(KW_GetGUI(widget));
}

void KW_GetWidgetAbsoluteGeometry(KW_Widget * widget, SDL_Rect * geometry) {
  *geometry = widget->absolute;
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

/* recursively calculate absolute geometry */
void CalculateAbsoluteGeometry(KW_Widget * widget) {
  int i = 0;
  SDL_Rect oldabs = widget->absolute, parentabs;
  KW_GetWidgetAbsoluteGeometry(widget->parent, &parentabs); /* get parent absolute */
  widget->absolute.x = widget->geometry.x + parentabs.x;
  widget->absolute.y = widget->geometry.y + parentabs.y;
  widget->absolute.w = widget->geometry.w;
  widget->absolute.h = widget->geometry.h;
  
  /* absolute changed, children absolute must also change */
  if (oldabs.x != widget->absolute.x || oldabs.y != widget->absolute.y) {
    for (i = 0; i < widget->childrencount; i++) {
      CalculateAbsoluteGeometry(widget->children[i]);
    }
  }
}

/* recursively calculate (childs->parent->parent->...->root) all composed geometries.
 * 
 * They are relative to its parent, thus, if a child has a .x coordinate less than
 * 0, it is outside the current widget box and it must be recalculated. The current
 * widget parent must also be notified of this change if we happend to be outside parent box */
void CalculateComposedGeometry(KW_Widget * widget) {
  
  SDL_Rect composed, edges;
  KW_Widget * children = NULL;
  int i = 0;
  if (widget->parent == NULL) return;
  if (widget->clipchildren) {
    widget->composed = widget->geometry;
    return;
  }
  
  edges = widget->geometry;
  for (i = 0; i < widget->childrencount; i++) {
    children = widget->children[i];
    KW_GetWidgetComposedGeometry(children, &composed);
    if (composed.x + composed.w > edges.w) edges.w = composed.x + composed.w;
    if (composed.y + composed.h > edges.h) edges.h = composed.y + composed.h;    
    if (composed.x < 0) edges.x += composed.x;
    if (composed.y < 0) edges.y += composed.y;
  }
  if (edges.x < widget->geometry.x) edges.w += widget->geometry.x - edges.x;
  if (edges.y < widget->geometry.y) edges.h += widget->geometry.y - edges.y;
  
  widget->composed = edges;
  /* see if our composite geometry change is relevant to parent */
  if (widget->parent == NULL) return;
  KW_GetWidgetComposedGeometry(widget->parent, &composed);
  if (((widget->composed.x < composed.x)
       || (widget->composed.y < composed.y)
       || (widget->composed.w + widget->composed.x != composed.w)
       || (widget->composed.h + widget->composed.y != composed.h)
      )
      ||
      (widget->parent->parent == NULL) /* always calculate root geometry */
     )
  {
      CalculateComposedGeometry(widget->parent);
  }
  /* TODO: callback for changed? */
}

void KW_GetWidgetComposedGeometry(KW_Widget * widget, SDL_Rect * composed) {
  *composed = widget->composed;
}


/* reparent version that allow us to parent to NULL */
void Reparent(KW_Widget * widget, KW_Widget * newparent) {
  int i = 0, j = -1;  
#if !defined(NDEBUG) && defined(DEBUG_GEOMETRY_CHANGE)  
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

    /* warn parent widget handlers that children changed */
    for (i = 0; i < wp->eventhandlers[KW_ON_CHILDRENCHANGE].count; i++) {
      KW_OnWidgetChildrenChange handler = (KW_OnWidgetChildrenChange) 
          wp->eventhandlers[KW_ON_CHILDRENCHANGE].handlers[i];
      handler(wp, KW_CHILDRENCHANGE_REMOVED, widget);
    }
    
    /* need to recalculate old parent area */
    CalculateComposedGeometry(widget);    
    CalculateAbsoluteGeometry(widget);
  }
  
  if (newparent != NULL) {
    newparent->childrencount++;
    newparent->children = realloc(newparent->children, newparent->childrencount * sizeof(KW_Widget *));
    newparent->children[newparent->childrencount-1] = widget;
    widget->parent = newparent;
    
    /* warn new parent widget handlers that children array changed */
    for (i = 0; i < newparent->eventhandlers[KW_ON_CHILDRENCHANGE].count; i++) {
      KW_OnWidgetChildrenChange handler = (KW_OnWidgetChildrenChange)
          newparent->eventhandlers[KW_ON_CHILDRENCHANGE].handlers[i];
      handler(newparent, KW_CHILDRENCHANGE_ADDED, widget);
    }
    
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
  SDL_Renderer * renderer = KW_GetWidgetRenderer(root);
  SDL_Rect cliprect, viewport;
#if !defined(NDEBUG) && defined(DEBUG_PRINT_GEOMETRY_RECTANGLE)
  SDL_Rect geom = root->composed;
  Uint8 r, g, b, a;
  float maxrgb;
  if (root->parent) {
    geom.x += root->parent->absolute.x;
    geom.y += root->parent->absolute.y;
  }
#endif  
  /* paint the root, then paint its childrens */
  if (root->paint != NULL) {
    root->paint(root);
  }
  
  if (root->clipchildren) {
    SDL_RenderGetClipRect(renderer, &(root->oldcliprect));
    SDL_RenderGetViewport(renderer, &viewport);
    cliprect = root->absolute;
    cliprect.x += viewport.x; cliprect.y += viewport.y;
    SDL_RenderSetClipRect(renderer, &cliprect);
  }

#if !defined(NDEBUG) && defined(DEBUG_PRINT_GEOMETRY_RECTANGLE)  
  SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
  maxrgb = (KW_GetGUI(root)->currentmouseover == root ? 255.0f : 100.0f);
  SDL_SetRenderDrawColor(renderer, (geom.x / (viewport.w * 1.0f) + 1) * maxrgb, (geom.y / (viewport.h * 1.0f)) * maxrgb, (geom.w + geom.h * 1.0f) / (viewport.w + viewport.h * 1.0f) * maxrgb, 128);
  SDL_RenderFillRect(renderer, &geom);
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
#endif
  for (i = 0; i < root->childrencount; i++) {
    KW_PaintWidget(root->children[i]);
  }
  
  /* restore cliprect */
  if (root->clipchildren) {
    SDL_RenderSetClipRect(renderer, &(root->oldcliprect));
  }
}

void KW_SetWidgetGeometry(KW_Widget * widget, const SDL_Rect * geometry) {
  int i;
  SDL_Rect old;
  KW_OnGeometryChange handler;
  if ((widget->geometry.x != geometry->x ||
      widget->geometry.y != geometry->y ||
      widget->geometry.w != geometry->w ||
      widget->geometry.h != geometry->h)
     || 
     (geometry->x == 0 && geometry->y == 0 && geometry->w == 0 && geometry->h == 0))
  {
    old = widget->geometry;
    widget->geometry = *geometry;
    CalculateComposedGeometry(widget);
    CalculateAbsoluteGeometry(widget);
    for (i = 0; i < widget->eventhandlers[KW_ON_GEOMETRYCHANGED].count; i++) {
      handler = (KW_OnGeometryChange) widget->eventhandlers[KW_ON_GEOMETRYCHANGED].handlers[i];
      handler(widget, geometry, &old);
    }
  }
}

void KW_GetWidgetGeometry(KW_Widget * widget, SDL_Rect * geometry) {
  *geometry = widget->geometry;
}


KW_GUI * KW_GetWidgetGUI(KW_Widget * widget) {
  return widget->gui;
}


void KW_AddWidgetDragHandler(KW_Widget * widget, KW_OnDrag handler) {
  AddWidgetHandler(widget, KW_ON_DRAG, (WidgetHandler) handler);
}

void KW_RemoveWidgetDragHandler(KW_Widget * widget, KW_OnDrag handler) {
  RemoveWidgetHandler(widget, KW_ON_DRAG, (WidgetHandler) handler);
}

void KW_AddWidgetDragStartHandler(KW_Widget * widget, KW_OnDragStart handler) {
  AddWidgetHandler(widget, KW_ON_DRAGSTART, (WidgetHandler) handler);
}

void KW_RemoveWidgetDragStartHandler(KW_Widget * widget, KW_OnDragStart handler) {
  RemoveWidgetHandler(widget, KW_ON_DRAGSTART, (WidgetHandler) handler);
}

void KW_AddWidgetDragStopHandler(KW_Widget * widget, KW_OnDragStop handler) {
  AddWidgetHandler(widget, KW_ON_DRAGSTOP, (WidgetHandler) handler);
}

void KW_RemoveWidgetDragStopHandler(KW_Widget * widget, KW_OnDragStop handler) {
  RemoveWidgetHandler(widget, KW_ON_DRAGSTOP, (WidgetHandler) handler);
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

void KW_AddWidgetGeometryChangeHandler(KW_Widget * widget, KW_OnGeometryChange handler) {
  AddWidgetHandler(widget, KW_ON_GEOMETRYCHANGED, (WidgetHandler) handler);
}

void KW_RemoveWidgetGeometryChangeHandler(KW_Widget * widget, KW_OnGeometryChange handler) {
  RemoveWidgetHandler(widget, KW_ON_GEOMETRYCHANGED, (WidgetHandler) handler);
}

void KW_AddWidgetChildrenChangeHandler(KW_Widget * widget, KW_OnWidgetChildrenChange handler) {
  AddWidgetHandler(widget, KW_ON_CHILDRENCHANGE, (WidgetHandler) handler);
}

void KW_RemoveWidgetChildrenChangeHandler(KW_Widget * widget, KW_OnWidgetChildrenChange handler) {
  RemoveWidgetHandler(widget, KW_ON_CHILDRENCHANGE, (WidgetHandler) handler);
}
