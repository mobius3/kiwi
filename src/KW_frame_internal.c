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
  KW_Frame * frame = KW_GetWidgetData(widget, PaintFrame);
  KW_ReleaseTexture(KW_GetWidgetRenderer(widget), frame->framerender);
  free(frame);
}


void RenderFrame(KW_Widget * widget) {
  KW_Frame * frame = KW_GetWidgetData(widget, PaintFrame);
  KW_Rect targetgeom;
  KW_Surface * tileset = KW_GetWidgetTilesetSurface(widget);
  KW_CustomRenderFunction renderfunc = KW_GetWidgetCustomRenderFunction(widget);
  KW_GetWidgetGeometry(widget, &targetgeom);
  if (frame->framerender != NULL) KW_ReleaseTexture(KW_GetWidgetRenderer(widget), frame->framerender);
  
  if (renderfunc != NULL)
    frame->framerender = renderfunc(KW_GetWidgetRenderer(widget), widget, tileset, targetgeom.w, targetgeom.h);
  else
    frame->framerender = KW_CreateTileFrameTexture(KW_GetWidgetRenderer(widget), tileset, 0, 0, targetgeom.w, targetgeom.h, KW_FALSE, KW_FALSE);
}

void PaintFrame(KW_Widget * widget, const KW_Rect * absolute, void * data) {
  KW_Frame * frame = (KW_Frame *) data;
  KW_Rect targetgeom = *absolute;
  KW_RenderDriver * renderer = KW_GetWidgetRenderer(widget);

  if (!KW_QueryWidgetHint(widget, KW_WIDGETHINT_ALLOWTILESTRETCH)) {
    KW_RenderCopy(renderer, frame->framerender, NULL, &targetgeom);
  } else {
    KW_RenderTileFrame(KW_GetWidgetRenderer(widget), KW_GetWidgetTilesetTexture(widget), 0, 0, &targetgeom, KW_TRUE, KW_TRUE);
  }
}


void FrameGeometryChanged(KW_Widget * widget, const KW_Rect * newgeom, const KW_Rect * oldgeom) {
  if (newgeom->w != oldgeom->w || newgeom->h != oldgeom->h) {
    RenderFrame(widget);
  }
}
