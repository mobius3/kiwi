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
static void TextInput(KW_Widget * widget, const char * text);
static void TextBackspace(KW_Widget * widget);
static void TextDelete(KW_Widget * widget);
static void KeyDown(KW_Widget * widget, SDL_Keycode key, SDL_Scancode sym);
static void KeyUp(KW_Widget * widget, SDL_Keycode key, SDL_Scancode sym);


KW_Widget * KW_CreateEditbox(KW_GUI * gui, KW_Widget * parent, const char * text, const SDL_Rect * geometry) {
  KW_Editbox * editbx = AllocEditbox();
  SDL_Rect labelgeom;
  KW_Widget * widget = KW_CreateWidget(gui, parent, KW_WIDGETTYPE_EDITBOX, geometry, PaintEditbox, DestroyEditbox, editbx);
  SDL_strlcat(editbx->text, text, 1024);
  editbx->cursor = SDL_strlen(text);
  
    
  labelgeom.x = TILESIZE;
  labelgeom.y = TILESIZE;
  labelgeom.w = geometry->w - TILESIZE * 2;
  labelgeom.h = geometry->h - TILESIZE * 2;
  
  editbx->labelwidget = KW_CreateLabel(gui, widget, text, &labelgeom);
  KW_SetLabelAlignment(editbx->labelwidget, KW_LABEL_ALIGN_LEFT, 0, KW_LABEL_ALIGN_MIDDLE, 0);
  KW_BlockWidgetInputEvents(editbx->labelwidget);
  KW_SetLabelCursor(editbx->labelwidget, editbx->cursor);
  
  KW_AddWidgetMouseOverHandler(widget, MouseOver);
  KW_AddWidgetMouseLeaveHandler(widget, MouseLeave);
  KW_AddWidgetMouseDownHandler(widget, MousePress);
  KW_AddWidgetMouseUpHandler(widget, MouseRelease);
  KW_AddWidgetFocusGainHandler(widget, FocusGain);
  KW_AddWidgetFocusLoseHandler(widget, FocusLose);
  KW_AddWidgetTextInputHandler(widget, TextInput);
  KW_AddWidgetKeyDownHandler(widget, KeyDown);
  KW_AddWidgetKeyUpHandler(widget, KeyUp);
  
  return widget;
}

void PaintEditbox(KW_Widget * widget) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
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

void AdjustCursor(KW_Widget * widget, int cursormove) {
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  int len = SDL_strlen(editbx->text);

  if (((cursormove > 0) && (editbx->cursor < len)) || ((cursormove < 0) && (editbx->cursor > 0)))
    editbx->cursor += cursormove;
  
  KW_SetLabelCursor(editbx->labelwidget, editbx->cursor);
}

void KeyDown(KW_Widget * widget, SDL_Keycode key, SDL_Scancode scan) {
  switch (scan) {
    /* set up cursor states */
    case SDL_SCANCODE_LEFT:
      AdjustCursor(widget, -1);
      break;
    case SDL_SCANCODE_RIGHT:
      AdjustCursor(widget, 1);
      break;
      
    case SDL_SCANCODE_BACKSPACE:
      TextBackspace(widget);
      break;
      
    case SDL_SCANCODE_DELETE:
      TextDelete(widget);
      break;
      
    default:
      break;
  }
}

void KeyUp(KW_Widget * widget, SDL_Keycode key, SDL_Scancode scan) {
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  switch (scan) {
    /* fall through to unset cursor walk */
    case SDL_SCANCODE_LEFT:
    case SDL_SCANCODE_RIGHT:
      break;
      
    default:
      break;
  }
}


void TextBackspace(KW_Widget * widget) {
  int i = 0;
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  if (editbx->cursor == 0) {
    return;
  }
  int len = SDL_strlen(editbx->text);
  

  for (i = editbx->cursor-1; i < len; i++) {
    editbx->text[i] = editbx->text[i+1];
  }
  AdjustCursor(widget, -1);
  KW_SetLabelText(editbx->labelwidget, editbx->text);
}

void TextDelete(KW_Widget * widget) {
}


static void TextInput(KW_Widget * widget, const char * text) {
  int i = 0, insertlen, textlen;
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  
  /* make room in the middle */
  i = editbx->cursor;
  insertlen = SDL_strlen(text);
  textlen = SDL_strlen(editbx->text);

  for (i = textlen; i >= editbx->cursor && i > 0; i--) {
    editbx->text[textlen+insertlen-(textlen-i)] = editbx->text[i];
  }
  
  for (i = 0; i < insertlen; i++) {
    editbx->text[editbx->cursor + i] = text[i];
  }
  editbx->cursor += insertlen;
  KW_SetLabelText(editbx->labelwidget, editbx->text);
  KW_SetLabelCursor(editbx->labelwidget, editbx->cursor);
}


static void MouseOver(KW_Widget * widget) {
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbx->mouseover = SDL_TRUE;
}


static void MouseLeave(KW_Widget * widget) {
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbx->mouseover = SDL_FALSE;
}

static void MousePress(KW_Widget * widget, int b) {
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbx->clicked = SDL_TRUE;
}

static void MouseRelease(KW_Widget * widget, int b) {
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbx->clicked = SDL_FALSE;
}

void FocusGain(KW_Widget * widget) {
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbx->active = SDL_TRUE;
  KW_ShowLabelCursor(editbx->labelwidget);
  SDL_StartTextInput();
}

void FocusLose(KW_Widget * widget) {
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbx->active = SDL_FALSE;
  KW_HideLabelCursor(editbx->labelwidget);
  editbx->cursormove = 0;
  SDL_RemoveTimer(editbx->cursortimer);  
  SDL_StopTextInput();
}


