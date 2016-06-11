#ifndef KW_CHECKBOX_H
#define KW_CHECKBOX_H

#include "KW_widget.h"

extern DECLSPEC KW_Widget * KW_CreateCheckbox(KW_GUI * gui, KW_Widget * parent, KW_Widget * label, const KW_Rect * geometry);
extern DECLSPEC KW_Widget * KW_GetCheckboxLabel(KW_Widget * widget);
extern DECLSPEC KW_Widget * KW_SetCheckboxLabel(KW_Widget * widget, KW_Widget * label);

#endif
