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
  SDL_DestroyTexture(frame->framerender);
  free(frame);
}


void RenderFrame(KW_Widget * widget) {
  KW_Frame * frame = KW_GetWidgetData(widget, KW_WIDGETTYPE_FRAME);
  SDL_Rect targetgeom;
  SDL_Surface * tileset = KW_GetWidgetTilesetSurface(widget);
  KW_GetWidgetGeometry(widget, &targetgeom);
  if (frame->framerender != NULL) SDL_DestroyTexture(frame->framerender);
  frame->framerender = KW_CreateTileFrameTexture(KW_GetWidgetRenderer(widget), tileset, 0, 0, targetgeom.w, targetgeom.h);
}

void PaintFrame(KW_Widget * widget) {
  KW_Frame * frame = KW_GetWidgetData(widget, KW_WIDGETTYPE_FRAME);
  SDL_Rect targetgeom;
  SDL_Renderer * renderer;
  
  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);
  renderer = KW_GetWidgetRenderer(widget);
  
  SDL_RenderCopy(renderer, frame->framerender, NULL, &targetgeom);
}


void FrameGeometryChanged(KW_Widget * widget, const SDL_Rect * newgeom, const SDL_Rect * oldgeom) {
  if (newgeom->w != oldgeom->w || newgeom->h != newgeom->h) {
    RenderFrame(widget);
  }
}
