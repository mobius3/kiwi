#ifndef KW_FRAME_INTERNAL_H
#define KW_FRAME_INTERNAL_H

#include "SDL.h"

typedef struct KW_Frame {
  SDL_Texture * framerender;
} KW_Frame;

KW_Frame * AllocFrame();
void RenderFrame(KW_Widget * widget);
void PaintFrame(KW_Widget * widget);

#endif
