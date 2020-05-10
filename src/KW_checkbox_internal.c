#include "KW_checkbox_internal.h"
#include "KW_label_internal.h"



void DestroyCheckbox(KW_Widget* widget){
  KW_Checkbox * checkbox = (KW_Checkbox *) KW_GetWidgetUserData(widget);
  DestroyLabel(widget);
  free(checkbox);
}
