#include "KW_button.h"
#include "KW_button_internal.h"
#include "KW_label.h"
#include "KW_tilerenderer.h"
#include "KW_gui.h"

KW_Button * AllocButton();
void PaintButton(KW_Widget * widget);
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
  labelgeom.x = TILESIZE;
  labelgeom.y = TILESIZE;
  labelgeom.w = geometry->w - TILESIZE * 2;
  labelgeom.h = geometry->h - TILESIZE * 2;
  
  KW_Button * button = AllocButton();
  KW_Widget * widget = KW_CreateWidget(gui, parent, KW_WIDGETTYPE_BUTTON, geometry, PaintButton, DestroyButton, button);
  button->labelwidget = KW_CreateLabel(gui, widget, text, &labelgeom);
  KW_BlockWidgetInputEvents(button->labelwidget);
  KW_AddWidgetMouseOverHandler(widget, MouseOver);
  KW_AddWidgetMouseLeaveHandler(widget, MouseLeave);
  KW_AddWidgetMouseDownHandler(widget, MousePress);
  KW_AddWidgetMouseUpHandler(widget, MouseRelease);
  return widget;
}

/** internal stuff **/

void PaintButton(KW_Widget * widget) {
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  /* base column for tile rendering */
  int basec = 0;
  if (button->mouseover) basec = 3;
  if (button->clicked) basec = 0;
  SDL_Rect targetgeom;
  SDL_Renderer * renderer;
  SDL_Texture * tileset;
  
  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);
  
  renderer = KW_GetWidgetRenderer(widget);
  tileset = KW_GetWidgetTileset(widget);
  
  KW_RenderTileFrame(renderer, tileset, 3, basec, targetgeom.x, targetgeom.y, targetgeom.w, targetgeom.h);
}

void DestroyButton(KW_Widget * widget) {
  
}


KW_Button * AllocButton() {
  KW_Button * button = calloc(sizeof(KW_Button), 1);
  return button;
}
