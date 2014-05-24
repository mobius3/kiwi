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
  Uint32 rmask, gmask, bmask, amask;  
  KW_GetWidgetGeometry(widget, &targetgeom);
  targetgeom.x = targetgeom.y = 0;

    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif

  target = SDL_CreateRGBSurface(0, targetgeom.w, targetgeom.h, 32, rmask, gmask, bmask, amask);  
  
  KW_BlitTileFrame(target, tileset, 0, 0, targetgeom.x, targetgeom.y, targetgeom.w, targetgeom.h);
  frame->framerender = SDL_CreateTextureFromSurface(KW_GetWidgetRenderer(widget), target);
}

void PaintFrame(KW_Widget * widget) {
  KW_Frame * frame = KW_GetWidgetData(widget, KW_WIDGETTYPE_FRAME);
  SDL_Rect targetgeom;
  SDL_Renderer * renderer;
  
  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);
  renderer = KW_GetWidgetRenderer(widget);
  
  SDL_RenderCopy(renderer, frame->framerender, NULL, &targetgeom);
}
