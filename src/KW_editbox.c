#include "KW_editbox.h"
#include "KW_widget.h"
#include "KW_gui.h"
#include "KW_editbox_internal.h"
#include "KW_tilerenderer.h"
#include "KW_textrenderer.h"

void PaintEditbox(KW_Widget * widget);
void RenderEditboxText(KW_Widget * widget);
void PaintEditboxText(KW_Widget * widget, SDL_Rect * geom);
void DestroyEditbox(KW_Widget * widget);
void AdjustCursor(KW_Widget * widget, int cursormove);
KW_Editbox * AllocEditbox();

static void MouseOver(KW_Widget * widget);
static void MouseLeave(KW_Widget * widget);
static void MousePress(KW_Widget * widget, int b);
static void MouseRelease(KW_Widget * widget, int b);
static void FocusGain(KW_Widget * widget);
static void FocusLose(KW_Widget * widget);
static void TextInput(KW_Widget * widget, const char * text);
static void TextBackspace(KW_Widget * widget);
static void TextDelete(KW_Widget * widget);
static void KeyDown(KW_Widget * widget, SDL_Keycode key, SDL_Scancode sym);
static void KeyUp(KW_Widget * widget, SDL_Keycode key, SDL_Scancode sym);

/* public functions */
KW_Widget * KW_CreateEditbox(KW_GUI * gui, KW_Widget * parent, const char * text, const SDL_Rect * geometry) {
  KW_Editbox * editbx = AllocEditbox();
  KW_Widget * widget = KW_CreateWidget(gui, parent, KW_WIDGETTYPE_EDITBOX, geometry, PaintEditbox, DestroyEditbox, editbx);
  SDL_strlcat(editbx->text, text, 1024);
  editbx->font = KW_GetFont(gui);


  KW_AddWidgetMouseOverHandler(widget, MouseOver);
  KW_AddWidgetMouseLeaveHandler(widget, MouseLeave);
  KW_AddWidgetMouseDownHandler(widget, MousePress);
  KW_AddWidgetMouseUpHandler(widget, MouseRelease);
  KW_AddWidgetFocusGainHandler(widget, FocusGain);
  KW_AddWidgetFocusLoseHandler(widget, FocusLose);
  KW_AddWidgetTextInputHandler(widget, TextInput);
  KW_AddWidgetKeyDownHandler(widget, KeyDown);
  KW_AddWidgetKeyUpHandler(widget, KeyUp);

  RenderEditboxText(widget);
  AdjustCursor(widget, SDL_strlen(text));

  return widget;
}

unsigned int KW_GetEditboxCursorPosition(KW_Widget * widget) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  return editbox->cursor;
}

TTF_Font * KW_GetEditboxFont(KW_Widget * widget) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  return editbox->font;
}

const char * KW_GetEditboxText(KW_Widget * widget) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  return editbox->text;
}

void KW_SetEditboxCursorPosition(KW_Widget * widget, unsigned int pos) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbox->cursor = pos;
}

void KW_SetEditboxFont(KW_Widget * widget, TTF_Font * font) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbox->font = font;
}

void KW_SetEditboxText(KW_Widget * widget, const char * text) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  SDL_strlcat(editbox->text, text, 1024);
  RenderEditboxText(widget);
}

/* private functions */
void PaintEditbox(KW_Widget * widget) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  /* base column for tile rendering */
  int basec = 0;
  if (editbox->mouseover || editbox->active) basec = 3;
  if (editbox->clicked) basec = 0;
  SDL_Rect targetgeom;
  SDL_Renderer * renderer;
  SDL_Texture * tileset;

  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);

  renderer = KW_GetWidgetRenderer(widget);
  tileset = KW_GetTileset(KW_GetGUI(widget));

  KW_RenderTileFrame(renderer, tileset, 6, basec, targetgeom.x, targetgeom.y, targetgeom.w, targetgeom.h);

  /* adjust target geom */
  targetgeom.x += TILESIZE;
  targetgeom.y += TILESIZE;
  targetgeom.w = targetgeom.w - TILESIZE * 2;
  targetgeom.h = targetgeom.h - TILESIZE * 2;

  PaintEditboxText(widget, &targetgeom);
}

/* paints the editbox text */
void PaintEditboxText(KW_Widget * widget, SDL_Rect * _dst) {
  KW_Editbox * editbx = (KW_Editbox *) KW_GetWidgetData(widget,
                        KW_WIDGETTYPE_EDITBOX);

  SDL_Rect dst = *_dst; // destination rectangle
  SDL_Rect orig = dst; // intended dst rectangle
  SDL_Rect src;        // text clipping

  SDL_Renderer * renderer = KW_GetWidgetRenderer(widget);
  SDL_Texture * tileset = KW_GetWidgetTileset(widget);

  /* query actual w and h */
  src.x = src.y = 0; src.w = editbx->textwidth; src.h = editbx->textheight;

  /* will paint label in the middle */
  dst.y = dst.y + dst.h / 2 - src.h / 2;

  // clip right if overflows


  // clip bottom part (middle, top)
  if (dst.y + src.h > orig.y + orig.h) src.h = orig.h + (orig.y - dst.y) - src.y;

  /* ajust text src rect and render cursor if active */
  int cursorx = editbx->cursorx;
  if (editbx->active) {
    int adjust = editbx->cursoradjustx;
    /* if the cursor is left to the clipping rectangle */
    if (cursorx < adjust) {
      adjust = cursorx;
    }
    /* if the cursor is right of the clipping rectangle (dst.w or less) */
    else if (cursorx > adjust + dst.w) {
      adjust = cursorx - dst.w;
      cursorx = adjust;
    }

    /* sets the clipping origin as calculated */
    src.x = adjust;
    
    /* save the adjust for next time */
    editbx->cursoradjustx = adjust;
    
    /* see if the clip rect is bigger than dst */
    if (src.w > dst.w) src.w = dst.w;
    
    /* see if clipping rect is bigger than total width, might have
     * to adjust clipping origin */
    if ((src.x + src.w) > editbx->textwidth) {
      src.x -= (src.x + src.w) - editbx->textwidth;
    }
    
    /* basic checks */
    if (src.w < dst.w) dst.w = src.w;
    if (src.h < dst.h) dst.h = src.h;

    /* render cursor *below* text */
    KW_RenderTileFill(renderer, tileset, 7, 7, dst.x + cursorx - src.x, dst.y,
                      TILESIZE, dst.h);
    
    /* render text */
    SDL_RenderCopy(renderer, editbx->textrender, &src, &dst);
  } else {
    /* just the basic checks */
    if (src.w > dst.w) src.w = dst.w;
    if (src.w < dst.w) dst.w = src.w;
    if (src.h < dst.h) dst.h = src.h;
    SDL_RenderCopy(renderer, editbx->textrender, &src, &dst);
  }
}


void RenderEditboxText(KW_Widget * widget) {
  KW_Editbox * editbx;
  editbx = (KW_Editbox *) KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  if (editbx->textrender != NULL) {
    SDL_DestroyTexture(editbx->textrender);
  }
  /* use our own font */
  editbx->textrender = KW_RenderTextLine(KW_GetEditboxFont(widget),
                                         KW_GetWidgetRenderer(widget),
                                         editbx->text, editbx->color,
                                         TTF_STYLE_NORMAL);

  if (editbx->textrender != NULL)
    SDL_QueryTexture(editbx->textrender, NULL, NULL, &(editbx->textwidth),
                     &(editbx->textheight));
  else
    TTF_SizeUTF8(editbx->font, "", &(editbx->textwidth), &(editbx->textheight));
}

KW_Editbox * AllocEditbox() {
  KW_Editbox * editbx = calloc(sizeof(*editbx), 1);
  return editbx;
}

void DestroyEditbox(KW_Widget * widget) {

}

void AdjustCursor(KW_Widget * widget, int cursormove) {
  char save;
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  int len = SDL_strlen(editbx->text);

  // adjust cursormove, don't let it over/underflow.
  if (cursormove > 0) {
    if (editbx->cursor + cursormove > len) {
      cursormove = len - editbx->cursor;
    }
  } else if (cursormove < 0) {
    if (-cursormove > editbx->cursor) {
      cursormove = -editbx->cursor;
    }
  }

  editbx->cursor += cursormove;

  /* recalculate cursor position */
  save = editbx->text[editbx->cursor];
  editbx->text[editbx->cursor] = '\0';
  TTF_SizeUTF8(editbx->font, editbx->text, &editbx->cursorx, NULL);
  editbx->text[editbx->cursor] = save;
}

void KeyDown(KW_Widget * widget, SDL_Keycode key, SDL_Scancode scan) {
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);  
  switch (scan) {
      /* set up cursor states */
    case SDL_SCANCODE_LEFT:
      AdjustCursor(widget, -1);
      break;
    case SDL_SCANCODE_RIGHT:
      AdjustCursor(widget, 1);
      break;

    case SDL_SCANCODE_BACKSPACE:
      TextBackspace(widget);
      break;

    case SDL_SCANCODE_DELETE:
      TextDelete(widget);
      break;

    case SDL_SCANCODE_HOME:
      AdjustCursor(widget, -SDL_strlen(editbx->text));
      break;
      
    case SDL_SCANCODE_END:
      AdjustCursor(widget, SDL_strlen(editbx->text));
      break;
      
    default:
      break;
  }
}

void KeyUp(KW_Widget * widget, SDL_Keycode key, SDL_Scancode scan) {
  //KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  switch (scan) {
      /* fall through to unset cursor walk */
    case SDL_SCANCODE_LEFT:
    case SDL_SCANCODE_RIGHT:
      break;

    default:
      break;
  }
}


void TextBackspace(KW_Widget * widget) {
  AdjustCursor(widget, -1);
  TextDelete(widget);
}

void TextDelete(KW_Widget * widget) {
  int i = 0;
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  if (editbx->cursor == 0) {
    return;
  }
  int len = SDL_strlen(editbx->text);

  for (i = editbx->cursor; i < len; i++) {
    editbx->text[i] = editbx->text[i + 1];
  }
  RenderEditboxText(widget);
}


static void TextInput(KW_Widget * widget, const char * text) {
  int i = 0, insertlen, textlen;
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);

  /* make room in the middle */
  i = editbx->cursor;
  insertlen = SDL_strlen(text);
  textlen = SDL_strlen(editbx->text);

  for (i = textlen; i >= editbx->cursor && i > 0; i--) {
    editbx->text[textlen + insertlen - (textlen - i)] = editbx->text[i];
  }

  for (i = 0; i < insertlen; i++) {
    editbx->text[editbx->cursor + i] = text[i];
  }
  AdjustCursor(widget, insertlen);
  RenderEditboxText(widget);
}


static void MouseOver(KW_Widget * widget) {
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbx->mouseover = SDL_TRUE;
}


static void MouseLeave(KW_Widget * widget) {
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbx->mouseover = SDL_FALSE;
}

static void MousePress(KW_Widget * widget, int b) {
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbx->clicked = SDL_TRUE;
}

static void MouseRelease(KW_Widget * widget, int b) {
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbx->clicked = SDL_FALSE;
}

void FocusGain(KW_Widget * widget) {
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbx->active = SDL_TRUE;
  SDL_StartTextInput();
}

void FocusLose(KW_Widget * widget) {
  KW_Editbox * editbx = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbx->active = SDL_FALSE;
  SDL_StopTextInput();
}


