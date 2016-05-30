#include "KW_widget.h"
#include "KW_tilerenderer.h"
#include "KW_frame_internal.h"
#include "KW_gui.h"
#include "SDL.h"

KW_Frame * AllocFrame() {
  KW_Frame * frame = calloc(sizeof(*frame), 1);
  return frame;
}

void DestroyFrame(KW_Widget * widget) {
  KW_Frame * frame = KW_GetWidgetData(widget, KW_WIDGETTYPE_FRAME);
  KW_ReleaseTexture(KW_GetWidgetRenderer(widget), frame->framerender);
  free(frame);
}


void RenderFrame(KW_Widget * widget) {
  KW_Frame * frame = KW_GetWidgetData(widget, KW_WIDGETTYPE_FRAME);
  KW_Rect targetgeom;
  KW_Surface * tileset = KW_GetWidgetTilesetSurface(widget);
  KW_GetWidgetGeometry(widget, &targetgeom);
  if (frame->framerender != NULL) KW_ReleaseTexture(KW_GetWidgetRenderer(widget), frame->framerender);
  frame->framerender = KW_CreateTileFrameTexture(KW_GetWidgetRenderer(widget), tileset, 0, 0, targetgeom.w, targetgeom.h);
}

void PaintFrame(KW_Widget * widget) {
  KW_Frame * frame = KW_GetWidgetData(widget, KW_WIDGETTYPE_FRAME);
  KW_Rect targetgeom;
  KW_RenderDriver * renderer;
  
  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);
  renderer = KW_GetWidgetRenderer(widget);

  KW_RenderCopy(renderer, frame->framerender, NULL, &targetgeom);
}


void FrameGeometryChanged(KW_Widget * widget, const KW_Rect * newgeom, const KW_Rect * oldgeom) {
  if (newgeom->w != oldgeom->w || newgeom->h != oldgeom->h) {
    RenderFrame(widget);
  }
}
