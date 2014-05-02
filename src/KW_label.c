#include "SDL.h"
#include "KW_label.h"
#include "KW_label_internal.h"
#include "KW_textrenderer.h"
#include "KW_tilerenderer.h"
#include "KW_gui.h"


void PaintLabel(KW_Widget * widget);

void RenderLabelText(KW_Widget * widget) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget);
  if (label->textrender != NULL) {
    SDL_DestroyTexture(label->textrender);
  }
  /* use our own font */
  label->textrender = KW_RenderTextLine(KW_GetLabelFont(widget),
                                         KW_GetWidgetRenderer(widget),
                                         label->text, label->color, label->style);
}

void DestroyLabel(KW_Widget * widget) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget);
  free(label->text);
  SDL_DestroyTexture(label->textrender);
  free(label);
}

KW_Widget * KW_CreateLabel(KW_GUI * gui, KW_Widget * parent, const char * text, const SDL_Rect * geometry) {
  KW_Label * label = calloc(sizeof(KW_Label), 1);
  KW_Widget * widget = KW_CreateWidget(gui, parent, KW_WIDGETTYPE_LABEL, geometry, PaintLabel, DestroyLabel, label);
  KW_SetLabelFont(widget, KW_GetFont(gui));
  KW_SetLabelText(widget, text);
  KW_SetLabelAlignment(widget, KW_LABEL_ALIGN_CENTER, 0, KW_LABEL_ALIGN_MIDDLE, 0);
  return widget;
}

void KW_SetLabelAlignment(KW_Widget * widget, KW_LabelHorizontalAlignment halign, int hoffset, KW_LabelVerticalAlignment valign, int voffset) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget); 
  label->halign = halign;
  label->hoffset = hoffset;
  label->valign = valign;
  label->voffset = voffset;
}


void KW_SetLabelCursor(KW_Widget * widget, unsigned int pos) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget); 
  label->cursor = pos;
}

unsigned int KW_GetLabelCursor(KW_Widget * widget) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget); 
  return label->cursor;
}

void KW_ShowLabelCursor(KW_Widget * widget) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget); 
  label->showcursor = SDL_TRUE;
}

void KW_HideLabelCursor(KW_Widget * widget) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget); 
  label->showcursor = SDL_FALSE;
}




TTF_Font * KW_GetLabelFont(KW_Widget * widget) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget);
  return label->font;
}


void KW_SetLabelFont(KW_Widget * widget, TTF_Font * font) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget);
  label->font = font;
  RenderLabelText(widget);
}


void KW_SetLabelText(KW_Widget * widget, const char * text) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget);
  if (label->text != NULL) {
    free(label->text);
  }
  label->text = SDL_strdup(text);
  RenderLabelText(widget);
}

void KW_SetLabelStyle(KW_Widget * widget, int style) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget);
  label->style = style;
  RenderLabelText(widget);
}

void KW_SetLabelColor(KW_Widget * widget, SDL_Color color) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget);
  label->color = color;
  RenderLabelText(widget);
}


void PaintLabel(KW_Widget * widget) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget);
  
  SDL_Rect orig;
  SDL_Rect dst;
  SDL_Rect src;
  
  SDL_Renderer * renderer = KW_GetWidgetRenderer(widget);
  SDL_Texture * tileset = KW_GetWidgetTileset(widget);
  
  /* query actual w and h */
  src.x = src.y = 0;
  SDL_QueryTexture(label->textrender, NULL, NULL, &src.w, &src.h);

  /* calculate target x/y */  
  KW_GetWidgetAbsoluteGeometry(widget, &dst);
  orig = dst;
  int textw, texth;
  
  
  /* calculate x according to halign */
  switch (label->halign) {
    
    case KW_LABEL_ALIGN_RIGHT:
      dst.x = dst.x + dst.w - src.w;
      break;
      
    case KW_LABEL_ALIGN_CENTER:
      dst.x = dst.x + dst.w/2 - src.w/2;
      break;
      
    /* easiest. do nothing */
    case KW_LABEL_ALIGN_LEFT:
    default:
        break;
  };
  
  dst.x += label->hoffset;
  
  /* calculate y according to valign */
  switch (label->valign) {
    case KW_LABEL_ALIGN_BOTTOM:
      dst.y = dst.y + dst.h - src.h;
      break;
      
    case KW_LABEL_ALIGN_MIDDLE:
      dst.y = dst.y + dst.h/2 - src.h/2;
      break;
      
    case KW_LABEL_ALIGN_TOP:
    default:
      break;
  }
  
  dst.y += label->voffset;
  
  /* clip texture so that it doesnt overflow desired maximum geometry */
  if (dst.x < orig.x) src.x = orig.x - dst.x;
  if (dst.y < orig.y) src.y = orig.y - dst.y;
  if (dst.x + src.w > orig.x + orig.w) src.w = orig.w + (orig.x - dst.x) - src.x;
  if (dst.y + src.h > orig.y + orig.h) src.h = orig.h + (orig.y - dst.y) - src.y;
  else src.h = src.h - src.y;
  
  /* don stretch the image */
  dst.w = src.w;
  dst.h = src.h;
  dst.x += src.x;
  dst.y += src.y;
  SDL_RenderCopy(renderer, label->textrender, &src, &dst);
    
  if (label->showcursor) {
    TTF_SizeUTF8(KW_GetLabelFont(widget), label->text, &textw, &texth);
    KW_RenderTileFill(renderer, tileset, 7, 7, dst.x + textw, dst.y, TILESIZE, texth);
  }
}
