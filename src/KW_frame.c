#include "KW_gui.h"
#include "KW_frame.h"
#include "SDL.h"
#include "KW_tilerenderer.h"
#include "KW_frame_internal.h"

void PaintFrame(KW_Widget * frame);

KW_Widget * KW_CreateFrame(KW_GUI * gui, KW_Widget * parent, const SDL_Rect * geometry) {
  KW_Frame * frame = AllocFrame();
  KW_Widget * widget = KW_CreateWidget(gui, parent, KW_WIDGETTYPE_FRAME, geometry, PaintFrame, NULL, frame);
  KW_AddWidgetGeometryChangeHandler(widget, FrameGeometryChanged);
  RenderFrame(widget);
  return widget;
}

