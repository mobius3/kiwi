#include "GUI_tilerenderer.h"

void GUI_RenderTile(SDL_Renderer * renderer, SDL_Texture * tileset, int column, int line, int x, int y) {
  SDL_Rect clip;
  SDL_Rect target;
  clip.x = column * TILESIZE;
  clip.y = line * TILESIZE;
  clip.w = TILESIZE;
  clip.h = TILESIZE;
  
  target.x = x;
  target.y = y;
  target.w = TILESIZE;
  target.h = TILESIZE;
  
  SDL_RenderCopy(renderer, tileset, &clip, &target);
}

void GUI_RenderTileFill(SDL_Renderer * renderer, SDL_Texture * tileset, int column, int line, int x, int y, int w, int h) {
 
  int i;
  int j;
  
  SDL_Rect clip;
  SDL_Rect target;
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
      SDL_RenderCopy(renderer, tileset, &clip, &target);
      
      j += TILESIZE;
    }
    
    i += TILESIZE;
  }
  
}
