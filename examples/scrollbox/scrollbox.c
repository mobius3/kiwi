#include "SDL.h"
#include "KW_gui.h"
#include "KW_button.h"
#include "KW_scrollbox.h"
#include "SDL_image.h"

void Drag(KW_Widget * widget, int x, int y, int xrel, int yrel) {
  SDL_Rect g;
  KW_GetWidgetGeometry(widget, &g);
  g.x += xrel;
  g.y += yrel;
  KW_SetWidgetGeometry(widget, &g);
}

int main(int argc, char ** argv) {
  SDL_Window * window;
  SDL_Renderer * renderer;
  SDL_Surface * set;
  KW_GUI * gui;
  TTF_Font * font;
  SDL_Rect geometry, g2, g3;
  SDL_Color color = { 255, 255, 255, 128 };  
  KW_Widget * frame, * button, * alabel;
  int i;
  char buf[1024];
  
  /* initialize window and renderer */
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(320, 240, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 100, 100, 100, 1);
  TTF_Init();
  
  /* load tileset */

  set = IMG_Load("tileset-alloy.png");
  
  /* initialize gui */
  gui = KW_Init(renderer, set);
  font = TTF_OpenFont("SourceSansPro-Semibold.ttf", 12);
  TTF_SetFontHinting(font, TTF_HINTING_NONE);
  KW_SetFont(gui, font);

  geometry.x = 30; geometry.y = 30; geometry.w = 200; geometry.h = 200;
  g2 = geometry;
  g2.x = 10, g2.y = 40, g2.w = 100, g2.h = 30;
  g3.x = 10, g3.y = 10, g3.w = 100, g3.h = 30;
  
  frame = KW_CreateFrame(gui, NULL, &geometry);
  geometry.x = 0; geometry.y = 0;
  frame = KW_CreateScrollbox(gui, frame, &geometry);
  g3.y=0;
  for (i = 0; i < 4; i++) {
    sprintf(buf, "%dx%d+%dx%d", g3.x, g3.y, g3.w, g3.h);
    button = KW_CreateButton(gui, frame, buf, &g3);
    g3.y+=g2.h;
    KW_AddWidgetDragHandler(button, Drag);
  }
  
  g2.x = 0, g2.y = -5, g2.w = 200, g2.h = 20;
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
