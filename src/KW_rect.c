#include "KW_rect.h"

void KW_RectCenterInParent(const KW_Rect * parent, KW_Rect * inner) {
  KW_RectHorizontallyCenterInParent(parent, inner);
  KW_RectVerticallyCenterInParent(parent, inner);
}

void KW_RectVerticallyCenterInParent(const KW_Rect * parent, KW_Rect * inner) {
  inner->y = (parent->h/2) - (inner->h/2);
}

void KW_RectHorizontallyCenterInParent(const KW_Rect * parent, KW_Rect * inner) {
  inner->x = (parent->w/2) - (inner->w/2);
}

void KW_RectCalculateEnclosingRect(KW_Rect * const rects[], unsigned count, KW_Rect * outer) {
  unsigned i;
  outer->x = rects[0]->x;
  outer->y = rects[0]->y;
  outer->w = rects[0]->w;
  outer->h = rects[0]->h;

  for (i = 0; i < count; i++) {
    if (rects[i]->x < outer->x) { outer->x = rects[i]->x; }
    if (rects[i]->y < outer->y) { outer->y = rects[i]->y; }
    if (rects[i]->w > outer->w) { outer->w = rects[i]->w; }
    if (rects[i]->h > outer->h) { outer->h = rects[i]->h; };
  }
}

void KW_RectLayoutVertically(KW_Rect * rects[], unsigned count, int padding, KW_RectHorizontalAlignment align) {
  KW_Rect outer, *inner;
  unsigned i;
  int current;

  KW_RectCalculateEnclosingRect(rects, count, &outer);
  current = outer.y;

  for (i = 0; i < count; i++) {
    inner = rects[i];
    inner->y = current;
    current += inner->h + padding;
    switch (align) {
      case KW_RECT_ALIGN_LEFT:
        inner->x = outer.x;
        break;
      case KW_RECT_ALIGN_CENTER:
        KW_RectHorizontallyCenterInParent(&outer, inner);
        inner->x += outer.x;
        break;
      case KW_RECT_ALIGN_RIGHT:
        inner->x = outer.w - inner->w;
        break;
      default:
        break;
    }
  }
}

void KW_RectLayoutHorizontally(KW_Rect * rects[], unsigned count, int padding, KW_RectVerticalAlignment align) {
  KW_Rect outer, *inner;
  unsigned i;
  int current;

  KW_RectCalculateEnclosingRect(rects, count, &outer);
  current = outer.x;

  for (i = 0; i < count; i++) {
    inner = rects[i];
    inner->x = current + padding;
    current = inner->w + inner->x;
    switch (align) {
      case KW_RECT_ALIGN_TOP:
        inner->y = outer.y;
        break;
      case KW_RECT_ALIGN_MIDDLE:
        KW_RectVerticallyCenterInParent(&outer, inner);
        inner->y += outer.y;
        break;
      case KW_RECT_ALIGN_BOTTOM:
        inner->y = outer.h - inner->h;
        break;
      default:
        break;
    }
  }
}

void KW_RectFillParentVertically(const KW_Rect * parent, KW_Rect * rects[], unsigned weights[], unsigned count, int padding) {
  KW_Rect * inner;
  unsigned i, weight, total = 0, base = 0;
  int current = 0;

  for (i = 0; i < count; i++) total += weights[i];
  base = (parent->h - padding*(count+1))/total;
  for (i = 0; i < count; i++) {
    weight = weights[i];
    inner = rects[i];
    inner->y = current + padding;
    inner->h = base * weight;
    current = inner->y + inner->h;
  }
}

void KW_RectFillParentHorizontally(const KW_Rect * parent, KW_Rect * rects[], unsigned weights[], unsigned count, int padding,
                             KW_RectVerticalAlignment align) {
  KW_Rect * inner;
  unsigned i, weight, total = 0, base = 0;
  int current = 0;

  for (i = 0; i < count; i++) total += weights[i];
  base = (parent->w - padding*(count+1))/total;
  for (i = 0; i < count; i++) {
    weight = weights[i];
    inner = rects[i];
    inner->x = current + padding;
    inner->w = base * weight;
    current = inner->x + inner->w;
    switch (align) {
      case KW_RECT_ALIGN_TOP:
        inner->y = 0;
        break;
      case KW_RECT_ALIGN_MIDDLE:
        KW_RectVerticallyCenterInParent(parent, inner);
        break;
      case KW_RECT_ALIGN_BOTTOM:
        inner->y = parent->h - inner->h;
        break;
      default:
        break;
    }
  }
}


void KW_SetRect(KW_Rect * rect, int x, int y, int w, int h) {
  rect->x = x;
  rect->y = y;
  rect->w = w;
  rect->h = h;
}

void KW_CopyRect(const KW_Rect * src, KW_Rect * dst) {
  *dst = *src;
}

void KW_ZeroRect(KW_Rect * rect) {
  rect->x = rect->y = rect->w = rect->h = 0;
}

void KW_MarginRect(const KW_Rect * outer, KW_Rect * inner, int margin) {
  KW_SetRect(inner, margin, margin, outer->w - margin*2, outer->h - margin*2);
}



