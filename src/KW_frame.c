#include "KW_frame.h"
#include "KW_frame_internal.h"
#include "KW_gui.h"
#include "KW_tilerenderer.h"
#include "SDL.h"

void PaintFrame(KW_Widget * widget, const KW_Rect * absolute, void * data);

KW_Widget * KW_CreateFrame(KW_GUI * gui, KW_Widget * parent,
                           const KW_Rect * geometry) {
  KW_Frame *  frame = AllocFrame();
  KW_Widget * widget =
      KW_CreateWidget(gui, parent, geometry, PaintFrame, DestroyFrame, frame);
  KW_AddWidgetGeometryChangeHandler(widget, FrameGeometryChanged);
  KW_AddWidgetTilesetChangeHandler(widget, RenderFrame);
  RenderFrame(widget);
  return widget;
}
