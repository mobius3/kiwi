#include "KW_editbox.h"
#include "KW_widget.h"
#include "KW_gui.h"
#include "KW_editbox_internal.h"

/* public functions */
KW_Widget * KW_CreateEditbox(KW_GUI * gui, KW_Widget * parent, const char * text, const SDL_Rect * geometry) {
  KW_Editbox * editbox = AllocEditbox();
  KW_Widget * widget = KW_CreateWidget(gui, parent, KW_WIDGETTYPE_EDITBOX, geometry, PaintEditbox, DestroyEditbox, editbox);
  SDL_strlcat(editbox->text, text, 1024);
  editbox->font = NULL;
  editbox->widget = widget;

  KW_AddWidgetMouseOverHandler(widget, EditboxMouseOver);
  KW_AddWidgetMouseLeaveHandler(widget, EditboxMouseLeave);
  KW_AddWidgetMouseDownHandler(widget, EditboxMousePress);
  KW_AddWidgetMouseUpHandler(widget, EditboxMouseRelease);
  KW_AddWidgetFocusGainHandler(widget, EditboxFocusGain);
  KW_AddWidgetFocusLoseHandler(widget, EditboxFocusLose);
  KW_AddWidgetTextInputHandler(widget, EditboxTextInput);
  KW_AddWidgetTextEditHandler(widget, EditboxTextEdit);
  KW_AddWidgetKeyDownHandler(widget, EditboxKeyDown);
  
  KW_AddGUIFontChangedHandler(gui, EditboxFontChanged, widget);

  RenderEditboxText(editbox);
  AdjustCursor(editbox, SDL_strlen(text));
  editbox->selectcursor = editbox->cursor;
  return widget;
}

unsigned int KW_GetEditboxCursorPosition(KW_Widget * widget) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  return editbox->cursor;
}

TTF_Font * KW_GetEditboxFont(KW_Widget * widget) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  return editbox->font == NULL ? KW_GetFont(KW_GetGUI(widget)) : editbox->font;
}

const char * KW_GetEditboxText(KW_Widget * widget) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  return editbox->text;
}

void KW_SetEditboxCursorPosition(KW_Widget * widget, unsigned int pos) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbox->cursor = pos;
}

void KW_SetEditboxFont(KW_Widget * widget, TTF_Font * font) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbox->font = font;
  RenderEditboxText(editbox);
  AdjustCursor(editbox, 1);
}

void KW_SetEditboxText(KW_Widget * widget, const char * text) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  SDL_strlcat(editbox->text, text, 1024);
  RenderEditboxText(editbox);
}
