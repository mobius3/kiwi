#ifndef KW_LABEL_INTERNAL_H
#define KW_LABEL_INTERNAL_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "KW_label.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef
struct KW_Label {
  char * text;
  KW_Texture * textrender;
  KW_Color color;
  KW_Label_Style style;
  KW_Font * font;
  KW_LabelHorizontalAlignment halign;
  KW_LabelVerticalAlignment valign;
  int hoffset;
  int voffset;
  unsigned int cursor;
  KW_bool showcursor;
  KW_bool dirty;
  KW_bool colorset;

  
  unsigned textwidth;
  unsigned textheight;
  int cursorx;
  int cursory;
  KW_Rect iconclip;
  
  int x;
} KW_Label;

void PaintLabel(KW_Widget * widget, const KW_Rect * absolute, void * data);
void RenderLabelText(KW_Widget * widget);
void DestroyLabel(KW_Widget * widget);
void LabelFontChanged(KW_GUI * gui, void * data, KW_Font * font);

#ifdef __cplusplus
}
#endif

#endif
