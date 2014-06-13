#include "SDL.h"
#include "KW_gui.h"
#include "KW_button.h"
#include "KW_scrollbox.h"
#include "SDL_image.h"

int dragmode = 0;

void DragStart(KW_Widget * widget, int x, int y) {
  SDL_Rect g;
  KW_GetWidgetAbsoluteGeometry(widget, &g);
  if (x > g.x + g.w - 20 && y > g.y + g.h -20) dragmode = 1;
  else dragmode = 0;
}

void DragStop(KW_Widget * widget, int x, int y) {
}

void Drag(KW_Widget * widget, int x, int y, int xrel, int yrel) {
  SDL_Rect g;
  KW_GetWidgetGeometry(widget, &g);
  if (dragmode == 1) {
    g.w += xrel;
    g.h += yrel;
  } else {
    g.x += xrel;
    g.y += yrel;
  }
  KW_SetWidgetGeometry(widget, &g);
}

int main(int argc, char ** argv) {
  SDL_Window * window;
  SDL_Renderer * renderer;
  SDL_Surface * set;
  KW_GUI * gui;
  TTF_Font * font;
  SDL_Rect geometry, g3;
  KW_Widget * frame, * button;
  int i;
  
  /* initialize window and renderer */
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(1280, 768, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 100, 100, 100, 1);
  SDL_RenderSetLogicalSize(renderer, 1280, 768);
  TTF_Init();
  
  /* load tileset */

  set = IMG_Load("tileset.png");
  
  /* initialize gui */
  gui = KW_Init(renderer, set);
  font = TTF_OpenFont("SourceSansPro-Semibold.ttf", 12);
  TTF_SetFontHinting(font, TTF_HINTING_NONE);
  KW_SetFont(gui, font);

  geometry.x = 160; geometry.y = 121; geometry.w = 960; geometry.h = 526;
  g3.x = 10, g3.y = 10, g3.w = 100, g3.h = 30;
  
  frame =  NULL;
  frame = KW_CreateScrollbox(gui, frame, &geometry);
  KW_AddWidgetDragStartHandler(frame, DragStart);
  KW_AddWidgetDragHandler(frame, Drag);
  KW_AddWidgetDragStopHandler(frame, DragStop);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  g3.x = 0; g3.y = 0; g3.h = 150; g3.w = 150;
  
  for (i = 0; i < 10; i++) {
    g3.x = i * 40; 
    g3.y = i * 10;
    button = KW_CreateButton(gui, frame, "Button", &g3);
    KW_AddWidgetDragStartHandler(button, DragStart);
    KW_AddWidgetDragHandler(button, Drag);
    KW_AddWidgetDragStopHandler(button, DragStop);
  }
    
  /* create another parent frame */
  while (!SDL_QuitRequested()) {
    SDL_RenderClear(renderer);
    KW_Paint(gui);
    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  
  KW_Quit(gui);
  SDL_FreeSurface(set);
  SDL_Quit();
  
  return 0;
}
