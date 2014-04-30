#include "SDL.h"
#include "GUI_label.h"
#include "GUI_label_internal.h"
#include "GUI_textrenderer.h"
#include "GUI_gui.h"

void PaintLabel(GUI_Widget * widget);

void RenderLabelText(GUI_Widget * widget) {
  GUI_Label * label = (GUI_Label *) GUI_GetWidgetData(widget);
  if (label->textrender != NULL) {
    SDL_DestroyTexture(label->textrender);
  }
  /* use our own font */
  label->textrender = GUI_RenderTextLine(GUI_GetLabelFont(widget),
                                         GUI_GetWidgetRenderer(widget),
                                         label->text, label->color, label->style);
}

void DestroyLabel(GUI_Widget * widget) {
  GUI_Label * label = (GUI_Label *) GUI_GetWidgetData(widget);
  free(label->text);
  SDL_DestroyTexture(label->textrender);
  free(label);
}

GUI_Widget * GUI_CreateLabel(GUI_GUI * gui, GUI_Widget * parent, const char * text, const SDL_Rect * geometry) {
  GUI_Label * label = calloc(sizeof(GUI_Label), 1);
  GUI_Widget * widget = GUI_CreateWidget(gui, parent, GUI_WIDGETTYPE_LABEL, geometry, PaintLabel, DestroyLabel, label);
  GUI_SetLabelFont(widget, GUI_GetFont(gui));
  GUI_SetLabelText(widget, text);
  GUI_SetLabelAlignment(widget, LABEL_ALIGN_CENTER, 0, LABEL_ALIGN_MIDDLE, 0);
  return widget;
}

void GUI_SetLabelAlignment(GUI_Widget * widget, GUI_LabelHorizontalAlignment halign, int hoffset, GUI_LabelVerticalAlignment valign, int voffset) {
  GUI_Label * label = (GUI_Label *) GUI_GetWidgetData(widget); 
  label->halign = halign;
  label->hoffset = hoffset;
  label->valign = valign;
  label->voffset = voffset;
}


TTF_Font * GUI_GetLabelFont(GUI_Widget * widget) {
  GUI_Label * label = (GUI_Label *) GUI_GetWidgetData(widget);
  return label->font;
}


void GUI_SetLabelFont(GUI_Widget * widget, TTF_Font * font) {
  GUI_Label * label = (GUI_Label *) GUI_GetWidgetData(widget);
  label->font = font;
  RenderLabelText(widget);
}


void GUI_SetLabelText(GUI_Widget * widget, const char * text) {
  GUI_Label * label = (GUI_Label *) GUI_GetWidgetData(widget);
  if (label->text != NULL) {
    free(label->text);
  }
  label->text = SDL_strdup(text);
  RenderLabelText(widget);
}

void GUI_SetLabelStyle(GUI_Widget * widget, int style) {
  GUI_Label * label = (GUI_Label *) GUI_GetWidgetData(widget);
  label->style = style;
  RenderLabelText(widget);
}

void GUI_SetLabelColor(GUI_Widget * widget, SDL_Color color) {
  GUI_Label * label = (GUI_Label *) GUI_GetWidgetData(widget);
  label->color = color;
  RenderLabelText(widget);
}


void PaintLabel(GUI_Widget * widget) {
  GUI_Label * label = (GUI_Label *) GUI_GetWidgetData(widget);
  
  SDL_Rect orig;
  SDL_Rect dst;
  SDL_Rect src;
  
  SDL_Renderer * renderer = GUI_GetWidgetRenderer(widget);
  
  /* query actual w and h */
  src.x = src.y = 0;
  SDL_QueryTexture(label->textrender, NULL, NULL, &src.w, &src.h);

  /* calculate target x/y */  
  GUI_GetWidgetAbsoluteGeometry(widget, &dst);
  orig = dst;
  
  
  /* calculate x according to halign */
  switch (label->halign) {
    
    case LABEL_ALIGN_RIGHT:
      dst.x = dst.x + dst.w - src.w;
      break;
      
    case LABEL_ALIGN_CENTER:
      dst.x = dst.x + dst.w/2 - src.w/2;
      break;
      
    /* easiest. do nothing */
    case LABEL_ALIGN_LEFT:
    default:
        break;
  };
  
  dst.x += label->hoffset;
  
  /* calculate y according to valign */
  switch (label->valign) {
    case LABEL_ALIGN_BOTTOM:
      dst.y = dst.y + dst.h - src.h;
      break;
      
    case LABEL_ALIGN_MIDDLE:
      dst.y = dst.y + dst.h/2 - src.h/2;
      break;
      
    case LABEL_ALIGN_TOP:
    default:
      break;
  }
  
  dst.y += label->voffset;
  
  
  /* clip texture so that it doesnt overflow desired maximum geometry */
  if (dst.x < orig.x) src.x = orig.x - dst.x;
  if (dst.y < orig.y) src.y = orig.y - dst.y;
  if (dst.x + src.w > orig.x + orig.w) src.w = orig.w + (orig.x - dst.x) - src.x;
  if (dst.y + src.h > orig.y + orig.h) src.h = orig.h + (orig.y - dst.y) - src.y;
  
  /* don stretch the image */
  dst.w = src.w;
  dst.h = src.h;
  dst.x += src.x;
  dst.y += src.y;
  SDL_RenderCopy(renderer, label->textrender, &src, &dst);
}
