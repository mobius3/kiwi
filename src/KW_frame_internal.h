#ifndef KW_FRAME_INTERNAL_H
#define KW_FRAME_INTERNAL_H

#include "SDL.h"
#include "KW_widget.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct KW_Frame {
  KW_Texture * framerender;
} KW_Frame;

KW_Frame * AllocFrame();
void RenderFrame(KW_Widget * widget);
void PaintFrame(KW_Widget * widget, const KW_Rect * absolute, void * data);
void DestroyFrame(KW_Widget * widget);
void FrameGeometryChanged(KW_Widget * widget, const KW_Rect * newrect, const KW_Rect * old);

#ifdef __cplusplus
}
#endif

#endif
