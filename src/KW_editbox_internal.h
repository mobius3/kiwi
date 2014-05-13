#ifndef KW_EDITBOX_INTERNAL_H
#define KW_EDITBOX_INTERNAL_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "KW_widget.h"

typedef struct KW_Edtibox {
  KW_Widget * widget;         /* the widget we pertain to */
  
  char text[1024];            /* the actual text */
  SDL_Texture * textrender;   /* the rendered text */
  TTF_Font * font;            /* font used to render text */
  unsigned int cursor;        /* in which char index is the cursor */
  unsigned int selectcursor;  /* in which char index is the select cursor */
  
  SDL_bool mouseover;         /* is mouse over the editbox */
  SDL_bool clicked;           /* was the editbox clicked */
  SDL_bool active;            /* is the editbox active */
                             
  int textwidth;              /* total width of the rendered text */
  int textheight;             /* total height of the rendered text */
  int cursorx;                /* cursor x position inside text texture */
  int selectcursorx;          /* select cursor x position */
  int cursoradjustx;          /* ajudstment in src.x text texture */
  int selectx;
  SDL_Color color;            /* color used to render the text */
  
} KW_Editbox;


/* util functions */
void RenderEditboxText(KW_Editbox * editbox);
void PaintEditboxText(KW_Editbox * editbox, SDL_Rect * geom);
void AdjustCursor(KW_Editbox * editbox, int cursormove);
KW_Editbox * AllocEditbox();
void TextBackspace(KW_Editbox * editbox);
void TextDelete(KW_Editbox * editbox, int amount);

/* main callbacks */
void DestroyEditbox(KW_Widget * widget);
void PaintEditbox(KW_Widget * widget);

/* event callbacks */
void EditboxMouseOver(KW_Widget * widget);
void EditboxMouseLeave(KW_Widget * widget);
void EditboxMousePress(KW_Widget * widget, int b);
void EditboxMouseRelease(KW_Widget * widget, int b);
void EditboxFocusGain(KW_Widget * widget);
void EditboxFocusLose(KW_Widget * widget);
void EditboxTextInput(KW_Widget * widget, const char * text);
void EditboxTextEdit(KW_Widget * widget, const char * text);
void EditboxKeyDown(KW_Widget * widget, SDL_Keycode key, SDL_Scancode sym);
void EditboxFontChanged(KW_GUI * gui, void * priv, TTF_Font * font);


#endif
