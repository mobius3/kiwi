#include "KW_button.h"
#include "KW_button_internal.h"
#include "KW_label.h"
#include "KW_tilerenderer.h"
#include "KW_gui.h"

KW_Button * AllocButton();
void PaintButton(KW_Widget * widget);
void RenderButton(KW_Widget * widget);
void DestroyButton(KW_Widget * widget);

static void MouseOver(KW_Widget * widget) {
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  button->mouseover = SDL_TRUE;
}


static void MouseLeave(KW_Widget * widget) {
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  button->mouseover = SDL_FALSE;
}

static void MousePress(KW_Widget * widget, int b) {
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  button->clicked = SDL_TRUE;
}

static void MouseRelease(KW_Widget * widget, int b) {
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  button->clicked = SDL_FALSE;
}


KW_Widget * KW_CreateButton(KW_GUI * gui, KW_Widget * parent, const char * text, const SDL_Rect * geometry) {
  SDL_Rect labelgeom;
  KW_Widget * widget = NULL;
  KW_Button * button = NULL;
  labelgeom.x = TILESIZE;
  labelgeom.y = TILESIZE;
  labelgeom.w = geometry->w - TILESIZE * 2;
  labelgeom.h = geometry->h - TILESIZE * 2;
  
  button = AllocButton();
  widget = KW_CreateWidget(gui, parent, KW_WIDGETTYPE_BUTTON, geometry, PaintButton, DestroyButton, button);
  button->labelwidget = KW_CreateLabel(gui, widget, text, &labelgeom);
  KW_BlockWidgetInputEvents(button->labelwidget);
  KW_AddWidgetMouseOverHandler(widget, MouseOver);
  KW_AddWidgetMouseLeaveHandler(widget, MouseLeave);
  KW_AddWidgetMouseDownHandler(widget, MousePress);
  KW_AddWidgetMouseUpHandler(widget, MouseRelease);
  RenderButton(widget);
  return widget;
}

/** internal stuff **/

void PaintButton(KW_Widget * widget) {
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  SDL_Rect targetgeom;
  SDL_Renderer * renderer = KW_GetWidgetRenderer(widget);
  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);
  if (button->mouseover)
    SDL_RenderCopy(renderer, button->normal, NULL, &targetgeom);
  else
    SDL_RenderCopy(renderer, button->over, NULL, &targetgeom);
}

void RenderButton(KW_Widget * widget) {
  SDL_Rect targetgeom;
  SDL_Renderer * renderer;
  SDL_Surface * tileset = KW_GetWidgetTilesetSurface(widget);
  SDL_Surface * target = NULL;
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);  
  targetgeom.x = targetgeom.y = 0;
  target = SDL_CreateRGBSurface(0, targetgeom.w, targetgeom.h, 32, tileset->format->Rmask, tileset->format->Gmask, tileset->format->Bmask, tileset->format->Amask);  
  KW_BlitTileFrame(target, tileset, 3, 3, targetgeom.x, targetgeom.y, targetgeom.w, targetgeom.h);
  button->normal = SDL_CreateTextureFromSurface(KW_GetWidgetRenderer(widget), target);
  SDL_FreeSurface(target);
  target = SDL_CreateRGBSurface(0, targetgeom.w, targetgeom.h, 32, tileset->format->Rmask, tileset->format->Gmask, tileset->format->Bmask, tileset->format->Amask);  
  KW_BlitTileFrame(target, tileset, 3, 0, targetgeom.x, targetgeom.y, targetgeom.w, targetgeom.h);
  button->over = SDL_CreateTextureFromSurface(KW_GetWidgetRenderer(widget), target);
}

void DestroyButton(KW_Widget * widget) {
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  SDL_DestroyTexture(button->over);
  SDL_DestroyTexture(button->normal);
}


KW_Button * AllocButton() {
  KW_Button * button = calloc(sizeof(KW_Button), 1);
  return button;
}
