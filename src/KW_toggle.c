#include "KW_toggle.h"
#include "KW_tilerenderer.h"

typedef struct KW_Toggle {
  KW_bool pushed;
} KW_Toggle;

void PaintToggle(KW_Widget * widget, const KW_Rect * absolute, void * data);
void DestroyToggle(KW_Widget * widget);
void MouseOver(KW_Widget * widget);
void MouseLeave(KW_Widget * widget);
void MousePress(KW_Widget * widget, int b);
void MouseRelease(KW_Widget * widget, int b);
KW_Toggle * AllocToggle();


KW_Widget * KW_CreateToggle(KW_GUI * gui, KW_Widget * parent, const KW_Rect * geometry) {
  KW_Toggle * tg = AllocToggle();
  KW_Widget * widget = KW_CreateWidget(gui, parent, geometry, PaintToggle, DestroyToggle, tg);
  tg->pushed = KW_FALSE;
  KW_AddWidgetMouseUpHandler(widget, MouseRelease);
  return widget;
}

KW_bool KW_IsToggleChecked(KW_Widget * widget) {
  KW_Toggle * tg = KW_GetWidgetData(widget, PaintToggle);
  return tg->pushed;
}

void KW_SetToggleChecked(KW_Widget * widget, KW_bool checked) {
  KW_Toggle * tg = KW_GetWidgetData(widget, PaintToggle);
  tg->pushed = checked;
}

KW_Toggle * AllocToggle() {
  return calloc(sizeof(KW_Toggle), 1);
}

void PaintToggle(KW_Widget * widget, const KW_Rect * absolute, void * data) {
  KW_Toggle * tg = (KW_Toggle *) data;
  KW_Rect targetgeom = *absolute;
  int column = 0;
  KW_RenderDriver * renderer = KW_GetWidgetRenderer(widget);
  if (tg->pushed) column = 3;
  if (KW_IsCursorOverWidget(widget)) column += 6;
  KW_RenderTileFrame(renderer, KW_GetWidgetTilesetTexture(widget), 12, column, &targetgeom, KW_FALSE, KW_FALSE);
}
void DestroyToggle(KW_Widget * widget) {
  KW_Toggle * tg = KW_GetWidgetData(widget, PaintToggle);
  free(tg);
}

void MouseRelease(KW_Widget * widget, int b) {
  KW_Toggle * tg = KW_GetWidgetData(widget, PaintToggle);
  (void) b;
  tg->pushed = (KW_bool) !tg->pushed;
}

