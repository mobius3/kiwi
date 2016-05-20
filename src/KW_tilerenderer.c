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


void KW_RenderTileFill(KW_RenderDriver * renderer, KW_Texture * tileset, int column, int line, int x, int y, int w, int h) {
 
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

void KW_BlitTileFill(KW_RenderDriver * renderer, KW_Surface * dst, KW_Surface * tileset, int column, int line, int x, int y, int w, int h) {
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


void KW_RenderTileFrame(KW_RenderDriver * renderer, KW_Texture * tileset, int startcolumn, int startline, int x, int y, int w, int h) {
  /* fill with background */
  KW_RenderTileFill(renderer, tileset, startcolumn + 1, startline + 1, x + TILESIZE, y + TILESIZE, w - TILESIZE * 2, h - TILESIZE * 2);
  
  /* fill top */
  KW_RenderTileFill(renderer, tileset, startcolumn + 1, startline + 0, x + TILESIZE, y, w - TILESIZE * 2, TILESIZE);
  
  /* fill bottom */
  KW_RenderTileFill(renderer, tileset, startcolumn + 1, startline + 2, x + TILESIZE, y + h - TILESIZE, w - TILESIZE * 2, TILESIZE);
  
  /* fill left */
  KW_RenderTileFill(renderer, tileset, startcolumn + 0, startline + 1, x, y + TILESIZE, TILESIZE, h - TILESIZE * 2);
  
  /* fill right */
  KW_RenderTileFill(renderer, tileset, startcolumn + 2, startline + 1, x + (w - TILESIZE), y + TILESIZE, TILESIZE, h - TILESIZE * 2);
  
  /* render top left */
  KW_RenderTile(renderer, tileset, startcolumn + 0, startline + 0, x, y);
  
  /* render top right */
  KW_RenderTile(renderer, tileset, startcolumn + 2, startline + 0, x + w - TILESIZE, y);
  
  /* render bottom left */
  KW_RenderTile(renderer, tileset, startcolumn + 0, startline + 2, x, y + h - TILESIZE);
  
  /* render bottom right */
  KW_RenderTile(renderer, tileset, startcolumn + 2, startline + 2, x + (w - TILESIZE), y + h - TILESIZE);
}

void KW_BlitTileFrame(KW_RenderDriver * renderer, KW_Surface * dst, KW_Surface * tileset, int startcolumn, int startline, int x, int y, int w, int h) {
  /* fill with background */
  KW_BlitTileFill(renderer, dst, tileset, startcolumn + 1, startline + 1, x + TILESIZE, y + TILESIZE, w - TILESIZE * 2, h - TILESIZE * 2);
  
  /* fill top */
  KW_BlitTileFill(renderer, dst, tileset, startcolumn + 1, startline + 0, x + TILESIZE, y, w - TILESIZE * 2, TILESIZE);
  
  /* fill bottom */
  KW_BlitTileFill(renderer, dst, tileset, startcolumn + 1, startline + 2, x + TILESIZE, y + h - TILESIZE, w - TILESIZE * 2, TILESIZE);
  
  /* fill left */
  KW_BlitTileFill(renderer, dst, tileset, startcolumn + 0, startline + 1, x, y + TILESIZE, TILESIZE, h - TILESIZE * 2);
  
  /* fill right */
  KW_BlitTileFill(renderer, dst, tileset, startcolumn + 2, startline + 1, x + (w - TILESIZE), y + TILESIZE, TILESIZE, h - TILESIZE * 2);
  
  /* render top left */
  KW_BlitTile(renderer, dst, tileset, startcolumn + 0, startline + 0, x, y);
  
  /* render top right */
  KW_BlitTile(renderer, dst, tileset, startcolumn + 2, startline + 0, x + w - TILESIZE, y);
  
  /* render bottom left */
  KW_BlitTile(renderer, dst, tileset, startcolumn + 0, startline + 2, x, y + h - TILESIZE);
  
  /* render bottom right */
  KW_BlitTile(renderer, dst, tileset, startcolumn + 2, startline + 2, x + (w - TILESIZE), y + h - TILESIZE);
}

KW_Texture * KW_CreateTileFrameTexture(KW_RenderDriver * renderer, KW_Surface * tileset, int startcolumn, int startline, int w, int h) {
  KW_Surface * target;
  KW_Texture * result;
  target = KW_CreateSurface(renderer, (unsigned)w, (unsigned)h);
  KW_BlitTileFrame(renderer, target, tileset, startcolumn, startline, 0, 0, w, h);
  result = KW_CreateTexture(renderer, target);
  /* TODO: proper error handling */
  KW_ReleaseSurface(renderer, target);
  return result;
}

