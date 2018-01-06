#ifndef KW_EDITBOX_INTERNAL_H
#define KW_EDITBOX_INTERNAL_H

#include "SDL.h"
#include "KW_widget.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct KW_Edtibox {
  KW_Widget * widget;         /* the widget we pertain to */
  
  char text[1024];            /* the actual text */
  KW_Texture * textrender;   /* the rendered text */
  KW_Font * font;            /* font used to render text */
  unsigned int cursor;        /* in which char index is the cursor */
  KW_bool mouseover;         /* is mouse over the editbox */
  KW_bool clicked;           /* was the editbox clicked */
  KW_bool active;            /* is the editbox active */
  KW_bool colorset;           /* was the color set on this editbox */
                             
  unsigned textwidth;              /* total width of the rendered text */
  unsigned textheight;             /* total height of the rendered text */
  int cursorx;                /* cursor x position inside text texture */
  int cursoradjustx;          /* ajudstment in src.x text texture */
  KW_Color color;            /* color used to render the text */

  
} KW_Editbox;


/* util functions */
void RenderEditboxText(KW_Editbox * editbox);
void PaintEditboxText(KW_Editbox * editbox, KW_Rect * geom);
void AdjustCursor(KW_Editbox * editbox, int cursormove);
KW_Editbox * AllocEditbox();
void TextBackspace(KW_Editbox * editbox);
void TextDelete(KW_Editbox * editbox);

/* main callbacks */
void DestroyEditbox(KW_Widget * widget);
void PaintEditbox(KW_Widget * widget, const KW_Rect * absolute, void * data);

/* event callbacks */
void EditboxMouseOver(KW_Widget * widget);
void EditboxMouseLeave(KW_Widget * widget);
void EditboxMousePress(KW_Widget * widget, int b);
void EditboxMouseRelease(KW_Widget * widget, int b);
void EditboxFocusGain(KW_Widget * widget);
void EditboxFocusLose(KW_Widget * widget);
void EditboxTextInput(KW_Widget * widget, const char * text);
void EditboxKeyDown(KW_Widget * widget, SDL_Keycode key, SDL_Scancode sym);
void EditboxFontChanged(KW_GUI * gui, void * priv, KW_Font * font);

#ifdef __cplusplus
}
#endif

#endif
