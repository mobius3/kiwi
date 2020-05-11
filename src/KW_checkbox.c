#include "KW_checkbox.h"
#include "KW_checkbox_internal.h"
#include "KW_gui.h"
#include "KW_label.h"
#include "KW_label_internal.h"

void MouseUpCheckbox(KW_Widget * widget, int b) {
  KW_Checkbox * checkbox = (KW_Checkbox *) KW_GetWidgetData(widget, PaintLabel);
  if (b != 1) {
    return;
  }
  checkbox->checked = !checkbox->checked;
  if (checkbox->checked) {
    KW_SetLabelIcon(widget, &checkbox->selected);
  } else {
    KW_SetLabelIcon(widget, &checkbox->unselected);
  }
}

KW_bool KW_IsCheckboxChecked(KW_Widget * widget) {
  KW_Checkbox * checkbox = (KW_Checkbox *) KW_GetWidgetData(widget, PaintLabel);
  return checkbox->checked;
}

KW_Widget * KW_CreateCheckbox(KW_GUI * gui, KW_Widget * parent,
                              const char * text, const KW_Rect * geometry) {
  KW_Checkbox * checkbox = calloc(sizeof(KW_Checkbox), 1);
  KW_Widget *   widget = KW_CreateWidget(gui, parent, geometry, PaintLabel,
                                       DestroyCheckbox, checkbox);
  KW_SetLabelText(widget, text);
  KW_SetLabelAlignment(widget, KW_LABEL_ALIGN_LEFT, 0, KW_LABEL_ALIGN_MIDDLE,
                       0);
  KW_AddGUIFontChangedHandler(gui, LabelFontChanged, widget);
  KW_LabelDisableWrap(widget);
  KW_AddWidgetMouseUpHandler(widget, &MouseUpCheckbox);
  return widget;
}

void KW_CheckboxSetChecked(KW_Widget * widget, KW_Rect * Rect) {
  KW_Checkbox * checkbox = (KW_Checkbox *) KW_GetWidgetData(widget, PaintLabel);
  checkbox->selected = *Rect;
  if (checkbox->checked) {
    KW_SetLabelIcon(widget, &checkbox->selected);
  }
}
void KW_CheckboxSetUnchecked(KW_Widget * widget, KW_Rect * Rect) {
  KW_Checkbox * checkbox = (KW_Checkbox *) KW_GetWidgetData(widget, PaintLabel);
  checkbox->unselected = *Rect;
  if (!checkbox->checked) {
    KW_SetLabelIcon(widget, &checkbox->unselected);
  }
}
