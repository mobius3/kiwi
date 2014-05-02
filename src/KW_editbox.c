#include "KW_editbox.h"
#include "KW_widget.h"
#include "KW_gui.h"
#include "KW_editbox_internal.h"
#include "KW_tilerenderer.h"
#include "KW_textrenderer.h"

void PaintEditbox(KW_Widget * widget);
void DestroyEditbox(KW_Widget * widget);
KW_Editbox * AllocEditbox();

static void MouseOver(KW_Widget * widget);
static void MouseLeave(KW_Widget * widget);
static void MousePress(KW_Widget * widget, int b);
static void MouseRelease(KW_Widget * widget, int b);
static void FocusGain(KW_Widget * widget);
static void FocusLose(KW_Widget * widget);


KW_Widget * KW_CreateEditbox(KW_GUI * gui, KW_Widget * parent, const char * text, const SDL_Rect * geometry) {
  KW_Editbox * editbx = AllocEditbox();
  SDL_Rect labelgeom;
  KW_Widget * widget = KW_CreateWidget(gui, parent, KW_WIDGETTYPE_EDITBOX, geometry, PaintEditbox, DestroyEditbox, editbx);
  editbx->text = SDL_strdup(text);
  editbx->cursor = 0;
  
    
  labelgeom.x = TILESIZE;
  labelgeom.y = TILESIZE;
  labelgeom.w = geometry->w - TILESIZE * 2;
  labelgeom.h = geometry->h - TILESIZE * 2;
  
  editbx->labelwidget = KW_CreateLabel(gui, widget, text, &labelgeom);
  KW_SetLabelAlignment(editbx->labelwidget, KW_LABEL_ALIGN_LEFT, 0, KW_LABEL_ALIGN_MIDDLE, 0);
  KW_BlockWidgetInputEvents(editbx->labelwidget);
  
  KW_AddWidgetMouseOverHandler(widget, MouseOver);
  KW_AddWidgetMouseLeaveHandler(widget, MouseLeave);
  KW_AddWidgetMouseDownHandler(widget, MousePress);
  KW_AddWidgetMouseUpHandler(widget, MouseRelease);
  KW_AddWidgetFocusGainHandler(widget, FocusGain);
  KW_AddWidgetFocusLoseHandler(widget, FocusLose);
  
  return widget;
}

void PaintEditbox(KW_Widget * widget) {
  KW_Editbox * editbox = KW_GetWidgetData(widget);
  /* base column for tile rendering */
  int basec = 0;
  if (editbox->mouseover || editbox->active) basec = 3;
  if (editbox->clicked) basec = 0;
  SDL_Rect targetgeom;
  SDL_Renderer * renderer;
  SDL_Texture * tileset;
  
  
  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);
 
  renderer = KW_GetWidgetRenderer(widget);
  tileset = KW_GetTileset(KW_GetGUI(widget));
  
  KW_RenderTileFrame(renderer, tileset, 6, basec, targetgeom.x, targetgeom.y, targetgeom.w, targetgeom.h);
  /* todo: handle selection */
}


KW_Editbox * AllocEditbox() {
  KW_Editbox * editbx = calloc(sizeof(*editbx), 1);
  return editbx;
}

void DestroyEditbox(KW_Widget * widget) {

}

static void MouseOver(KW_Widget * widget) {
  KW_Editbox * editbx = KW_GetWidgetData(widget);
  editbx->mouseover = SDL_TRUE;
}


static void MouseLeave(KW_Widget * widget) {
  KW_Editbox * editbx = KW_GetWidgetData(widget);
  editbx->mouseover = SDL_FALSE;
}

static void MousePress(KW_Widget * widget, int b) {
  KW_Editbox * editbx = KW_GetWidgetData(widget);
  editbx->clicked = SDL_TRUE;
}

static void MouseRelease(KW_Widget * widget, int b) {
  KW_Editbox * editbx = KW_GetWidgetData(widget);
  editbx->clicked = SDL_FALSE;
}

void FocusGain(KW_Widget * widget) {
  KW_Editbox * editbx = KW_GetWidgetData(widget);
  editbx->active = SDL_TRUE;
  KW_ShowLabelCursor(editbx->labelwidget);
}

void FocusLose(KW_Widget * widget) {
  KW_Editbox * editbx = KW_GetWidgetData(widget);
  editbx->active = SDL_FALSE;
  KW_HideLabelCursor(editbx->labelwidget);
}


