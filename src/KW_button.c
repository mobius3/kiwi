#include "KW_button.h"
#include "KW_button_internal.h"
#include "KW_label.h"
#include "KW_tilerenderer.h"
#include "KW_gui.h"

KW_Button * AllocButton();
void PaintButton(KW_Widget * widget, const KW_Rect * absolute, void * data);
void RenderButton(KW_Widget * widget);
void DestroyButton(KW_Widget * widget);
void ButtonGeometryChanged(KW_Widget * widget, const KW_Rect * newgeom, const KW_Rect * oldgeom);

KW_Widget * KW_CreateButtonAndLabel(KW_GUI * gui, KW_Widget * parent, const char * text, const KW_Rect * geometry) {
  return KW_CreateButton(gui, parent, KW_CreateLabel(gui, NULL, text, geometry), geometry);
}

KW_Widget * KW_CreateButton(KW_GUI * gui, KW_Widget * parent, KW_Widget * label, const KW_Rect * geometry) {
  KW_Rect labelgeom;
  KW_Widget * widget = NULL;
  KW_Button * button = NULL;
  KW_MarginRect(geometry, &labelgeom, TILESIZE);

  button = AllocButton();
  widget = KW_CreateWidget(gui, parent, geometry, PaintButton, DestroyButton, button);
  if (label) {
    KW_ReparentWidget(label, widget);
    KW_SetWidgetGeometry(label, &labelgeom);
  } else {
    label = KW_CreateLabel(gui, widget, "", &labelgeom);
  }
  button->labelwidget = label;
  KW_BlockWidgetInputEvents(button->labelwidget);
  KW_AddWidgetGeometryChangeHandler(widget, ButtonGeometryChanged);
  KW_AddWidgetTilesetChangeHandler(widget, RenderButton);
  RenderButton(widget);
  return widget;
}

/** internal stuff **/
void PaintButton(KW_Widget * widget, const KW_Rect * absolute, void * data) {
  KW_Button * button = (KW_Button *) data;
  KW_RenderDriver * renderer = KW_GetWidgetRenderer(widget);
  /* don't draw frame if requested. */
  if (KW_QueryWidgetHint(widget, KW_WIDGETHINT_FRAMELESS) == KW_TRUE)
    return;

  if (KW_IsCursorOverWidget(widget))
    KW_RenderCopy(renderer, button->normal, NULL, absolute);
  else
    KW_RenderCopy(renderer, button->over, NULL, absolute);
}

void RenderButton(KW_Widget * widget) {
  KW_Rect targetgeom;
  KW_Surface * tileset = KW_GetWidgetTilesetSurface(widget);
  KW_Button * button = KW_GetWidgetData(widget, PaintButton);
  KW_RenderDriver * driver = KW_GetWidgetRenderer(widget);
  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);  
  if (button->normal != NULL) KW_ReleaseTexture(driver, button->normal);
  button->normal = KW_CreateTileFrameTexture(KW_GetWidgetRenderer(widget), tileset, 3, 3, targetgeom.w, targetgeom.h, KW_FALSE, KW_FALSE);
  if (button->over != NULL) KW_ReleaseTexture(driver, button->over);
  button->over = KW_CreateTileFrameTexture(driver, tileset, 3, 0, targetgeom.w, targetgeom.h, KW_FALSE, KW_FALSE);
}

void DestroyButton(KW_Widget * widget) {
  KW_Button * button = KW_GetWidgetData(widget, PaintButton);
  KW_RenderDriver * driver = KW_GetWidgetRenderer(widget);
  KW_ReleaseTexture(driver, button->over);
  KW_ReleaseTexture(driver, button->normal);
  free(button);
}

void ButtonGeometryChanged(KW_Widget * widget, const KW_Rect * newgeom, const KW_Rect * oldgeom) {
  KW_Rect labelgeom;
  KW_Button * button = KW_GetWidgetData(widget, PaintButton);
  
  if (oldgeom->w != newgeom->w || oldgeom->h != newgeom->h) {
    RenderButton(widget);
  }
  
  labelgeom.x = TILESIZE;
  labelgeom.y = TILESIZE;
  labelgeom.w = newgeom->w - TILESIZE * 2;
  labelgeom.h = newgeom->h - TILESIZE * 2;
  KW_SetWidgetGeometry(button->labelwidget, &labelgeom);
}


KW_Button * AllocButton() {
  KW_Button * button = calloc(sizeof(KW_Button), 1);
  return button;
}

KW_Widget * KW_GetButtonLabel(KW_Widget * widget) {
  return ((KW_Button *)KW_GetWidgetData(widget, PaintButton))->labelwidget;
}

KW_Widget * KW_SetButtonLabel(KW_Widget * widget, KW_Widget * label) {
  KW_Button * button = KW_GetWidgetData(widget, PaintButton);
  KW_Widget * old = button->labelwidget;
  KW_Rect labelgeom;
  KW_ReparentWidget(old, NULL);
  KW_ReparentWidget(label, widget);
  KW_MarginRect(KW_ReturnWidgetGeometry(widget), &labelgeom, TILESIZE);
  KW_SetWidgetGeometry(label, &labelgeom);
  button->labelwidget = label;
  return old;
}