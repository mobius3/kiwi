#include "KW_widget.h"
#include "KW_tilerenderer.h"
#include "KW_frame_internal.h"
#include "KW_gui.h"
#include "SDL.h"

KW_Frame * AllocFrame() {
  KW_Frame * frame = calloc(sizeof(*frame), 1);
  return frame;
}

void RenderFrame(KW_Widget * widget) {
  KW_Frame * frame = KW_GetWidgetData(widget, KW_WIDGETTYPE_FRAME);
  SDL_Rect targetgeom;
  SDL_Surface * tileset = KW_GetWidgetTilesetSurface(widget);
  SDL_Surface * target = NULL;
  KW_GetWidgetGeometry(widget, &targetgeom);
  targetgeom.x = targetgeom.y = 0;

  target = SDL_CreateRGBSurface(0, targetgeom.w, targetgeom.h, 32, tileset->format->Rmask, tileset->format->Gmask, tileset->format->Bmask, tileset->format->Amask);  
  
  KW_BlitTileFrame(target, tileset, 0, 0, targetgeom.x, targetgeom.y, targetgeom.w, targetgeom.h);
  if (frame->framerender != NULL)
    SDL_DestroyTexture(frame->framerender);
  frame->framerender = SDL_CreateTextureFromSurface(KW_GetWidgetRenderer(widget), target);
  SDL_FreeSurface(target);
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
