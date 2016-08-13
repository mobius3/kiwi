#include "KW_tilerenderer.h"

void KW_RenderTile(KW_RenderDriver * renderer, KW_Texture * tileset, int column, int line, int x, int y) {
  KW_Rect clip;
  KW_Rect target;
  clip.x = column * TILESIZE;
  clip.y = line * TILESIZE;
  clip.w = TILESIZE;
  clip.h = TILESIZE;
  
  target.x = x;
  target.y = y;
  target.w = TILESIZE;
  target.h = TILESIZE;
  
  KW_RenderCopy(renderer, tileset, &clip, &target);
}

void KW_BlitTile(KW_RenderDriver * renderer, KW_Surface * dst, KW_Surface * tileset, int column, int line, int x, int y) {
  KW_Rect clip;
  KW_Rect target;
  clip.x = column * TILESIZE;
  clip.y = line * TILESIZE;
  clip.w = TILESIZE;
  clip.h = TILESIZE;
  
  target.x = x;
  target.y = y;
  target.w = TILESIZE;
  target.h = TILESIZE;
  
  KW_BlitSurface(renderer, tileset, &clip, dst, &target);
}


void KW_RenderTileFill(KW_RenderDriver * renderer, KW_Texture * tileset, int column, int line, int x, int y, int w, int h, KW_bool stretch) {
 
  int i;
  int j;
  
  KW_Rect clip;
  KW_Rect target;
  clip.x = column * TILESIZE;
  clip.y = line * TILESIZE;
  clip.w = TILESIZE;
  clip.h = TILESIZE;
  
  target.x = x;
  target.y = y;
  target.w = TILESIZE;
  target.h = TILESIZE;

  if (stretch) {
    target.w = w;
    target.h = h;
    KW_RenderCopy(renderer, tileset, &clip, &target);
    return;
  }

  /* iterate by column */
  for (i = x; i < x + w;) {
    
    /* set the target column */
    target.x = i;
    
    /* reset clipping */
    clip.w = TILESIZE;
    
    /* clip the width of the tile if it does not fit */
    if (i + TILESIZE > x + w) {
      clip.w = w % TILESIZE;
    }
    target.w = clip.w;
    
    /* iterate by lines in a column */
    for (j = y; j < y + h;) {
      /* set target line */
      target.y = j;
      
      /* reset clipping */
      clip.h = TILESIZE;
      
      /* clip tile heigh if it does not fit */
      if (j + TILESIZE > y + h) {
        clip.h = h % TILESIZE;
      }
      target.h = clip.h;
      
      /* finaly render it */
      KW_RenderCopy(renderer, tileset, &clip, &target);
      
      j += TILESIZE;
    }
    
    i += TILESIZE;
  }
}

void KW_BlitTileFill(KW_RenderDriver * renderer, KW_Surface * dst, KW_Surface * tileset, int column, int line, int x, int y, int w, int h, KW_bool stretch) {
  int i;
  int j;
  
  KW_Rect clip;
  KW_Rect target;
  clip.x = column * TILESIZE;
  clip.y = line * TILESIZE;
  clip.w = TILESIZE;
  clip.h = TILESIZE;
  
  target.x = x;
  target.y = y;
  target.w = TILESIZE;
  target.h = TILESIZE;

  if (stretch) {
    target.w = w;
    target.h = h;
    KW_BlitSurface(renderer, tileset, &clip, dst, &target);
    return;
  }

  /* iterate by column */
  for (i = x; i < x + w;) {
    
    /* set the target column */
    target.x = i;
    
    /* reset clipping */
    clip.w = TILESIZE;
    
    /* clip the width of the tile if it does not fit */
    if (i + TILESIZE > x + w) {
      clip.w = w % TILESIZE;
    }
    target.w = clip.w;
    
    /* iterate by lines in a column */
    for (j = y; j < y + h;) {
      /* set target line */
      target.y = j;
      
      /* reset clipping */
      clip.h = TILESIZE;
      
      /* clip tile heigh if it does not fit */
      if (j + TILESIZE > y + h) {
        clip.h = h % TILESIZE;
      }
      target.h = clip.h;
      
      /* finaly render it */
      KW_BlitSurface(renderer, tileset, &clip, dst, &target);
      
      j += TILESIZE;
    }
    
    i += TILESIZE;
  }
}


void KW_RenderTileFrame(KW_RenderDriver * renderer, KW_Texture * tileset, int startcolumn, int startline, const KW_Rect * fillrect, KW_bool stretchcenter, KW_bool stretchsides) {
  int x = fillrect->x, y = fillrect->y, w = fillrect->w, h = fillrect->h;

  /* fill with background */
  KW_RenderTileFill(renderer, tileset, startcolumn + 1, startline + 1, x + TILESIZE, y + TILESIZE, w - TILESIZE * 2, h - TILESIZE * 2, stretchcenter);
  
  /* fill top */
  KW_RenderTileFill(renderer, tileset, startcolumn + 1, startline + 0, x + TILESIZE, y, w - TILESIZE * 2, TILESIZE, stretchsides);
  
  /* fill bottom */
  KW_RenderTileFill(renderer, tileset, startcolumn + 1, startline + 2, x + TILESIZE, y + h - TILESIZE, w - TILESIZE * 2, TILESIZE, stretchsides);
  
  /* fill left */
  KW_RenderTileFill(renderer, tileset, startcolumn + 0, startline + 1, x, y + TILESIZE, TILESIZE, h - TILESIZE * 2, stretchsides);
  
  /* fill right */
  KW_RenderTileFill(renderer, tileset, startcolumn + 2, startline + 1, x + (w - TILESIZE), y + TILESIZE, TILESIZE, h - TILESIZE * 2, stretchsides);
  
  /* render top left */
  KW_RenderTile(renderer, tileset, startcolumn + 0, startline + 0, x, y);
  
  /* render top right */
  KW_RenderTile(renderer, tileset, startcolumn + 2, startline + 0, x + w - TILESIZE, y);
  
  /* render bottom left */
  KW_RenderTile(renderer, tileset, startcolumn + 0, startline + 2, x, y + h - TILESIZE);
  
  /* render bottom right */
  KW_RenderTile(renderer, tileset, startcolumn + 2, startline + 2, x + (w - TILESIZE), y + h - TILESIZE);
}

void KW_BlitTileFrame(KW_RenderDriver * renderer, KW_Surface * dst, KW_Surface * tileset, int startcolumn, int startline, const KW_Rect * fillrect, KW_bool stretchcenter, KW_bool stretchsides) {
  int x = fillrect->x, y = fillrect->y, w = fillrect->w, h = fillrect->h;
  KW_bool stretchleft, stretchright, stretchtop, stretchbottom;

  /* fill with background */
  stretchcenter = stretchcenter && KW_IsTileStretchable(renderer, tileset, startcolumn + 1, startline + 1);
  KW_BlitTileFill(renderer, dst, tileset, startcolumn + 1, startline + 1, x + TILESIZE, y + TILESIZE, w - TILESIZE * 2, h - TILESIZE * 2, stretchcenter);
  
  /* fill top */
  stretchtop = stretchsides && KW_IsTileStretchable(renderer, tileset, startcolumn + 1, startline + 0);
  KW_BlitTileFill(renderer, dst, tileset, startcolumn + 1, startline + 0, x + TILESIZE, y, w - TILESIZE * 2, TILESIZE, stretchtop);
  
  /* fill bottom */
  stretchbottom = stretchsides && KW_IsTileStretchable(renderer, tileset, startcolumn + 1, startline + 2);
  KW_BlitTileFill(renderer, dst, tileset, startcolumn + 1, startline + 2, x + TILESIZE, y + h - TILESIZE, w - TILESIZE * 2, TILESIZE, stretchbottom);
  
  /* fill left */
  stretchleft = stretchsides && KW_IsTileStretchable(renderer, tileset, startcolumn + 0, startline + 1);
  KW_BlitTileFill(renderer, dst, tileset, startcolumn + 0, startline + 1, x, y + TILESIZE, TILESIZE, h - TILESIZE * 2, stretchleft);
  
  /* fill right */
  stretchright = stretchsides && KW_IsTileStretchable(renderer, tileset, startcolumn + 2, startline + 1);
  KW_BlitTileFill(renderer, dst, tileset, startcolumn + 2, startline + 1, x + (w - TILESIZE), y + TILESIZE, TILESIZE, h - TILESIZE * 2, stretchright);
  
  /* render top left */
  KW_BlitTile(renderer, dst, tileset, startcolumn + 0, startline + 0, x, y);
  
  /* render top right */
  KW_BlitTile(renderer, dst, tileset, startcolumn + 2, startline + 0, x + w - TILESIZE, y);
  
  /* render bottom left */
  KW_BlitTile(renderer, dst, tileset, startcolumn + 0, startline + 2, x, y + h - TILESIZE);
  
  /* render bottom right */
  KW_BlitTile(renderer, dst, tileset, startcolumn + 2, startline + 2, x + (w - TILESIZE), y + h - TILESIZE);
}

KW_Texture * KW_CreateTileFrameTexture(KW_RenderDriver * renderer, KW_Surface * tileset, int startcolumn, int startline, int w, int h, KW_bool stretchcenter, KW_bool stretchsides) {
  KW_Surface * target;
  KW_Texture * result;
  KW_Rect fillrect;
  fillrect.x = 0; fillrect.y = 0; fillrect.w = w;  fillrect.h = h;
  target = KW_CreateSurface(renderer, (unsigned)w, (unsigned)h);
  KW_BlitTileFrame(renderer, target, tileset, startcolumn, startline, &fillrect, stretchcenter, stretchsides);
  result = KW_CreateTexture(renderer, target);
  /* TODO: proper error handling */
  KW_ReleaseSurface(renderer, target);
  return result;
}
KW_bool KW_IsTileStretchable(KW_RenderDriver * renderer, KW_Surface * tileset, int line, int column) {
  unsigned i = 0, j = 0;
  unsigned int lastpixel = KW_GetPixel(renderer, tileset, line * TILESIZE, column * TILESIZE);
  unsigned int pixel;
  KW_bool stretchable = KW_TRUE;
  for (i = 0; i < TILESIZE; ++i) {
    for (j = 0; j < TILESIZE; ++j) {
      pixel = KW_GetPixel(renderer, tileset, (line * TILESIZE) + i, column * TILESIZE + j);
      if (pixel != lastpixel) {
        stretchable = KW_FALSE;
        break;
      }
      lastpixel = pixel;
    }
  }
  return stretchable;
}

