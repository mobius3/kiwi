#include "KW_checkbox_internal.h"
#include "KW_label_internal.h"

void DestroyCheckbox(KW_Widget * widget) {
  KW_Checkbox * checkbox = (KW_Checkbox *) KW_GetWidgetData(widget, PaintLabel);
  SDL_free(checkbox->label.text);
  KW_ReleaseTexture(KW_GetWidgetRenderer(widget), checkbox->label.textrender);
  free(checkbox);
}
