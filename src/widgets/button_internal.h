#ifndef KW_BUTTON_INTERNAL_H
#define KW_BUTTON_INTERNAL_H

#include "core/widget.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct KW_Button {
  KW_Widget * labelwidget; /* the label inside the button frame */
  KW_Texture * normal;
  KW_Texture * over;
} KW_Button;

#ifdef __cplusplus
}
#endif

#endif
