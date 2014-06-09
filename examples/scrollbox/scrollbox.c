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
  printf("Drag has started at %dx%d\n", x, y);
}

void DragStop(KW_Widget * widget, int x, int y) {
  printf("Drag has stopped at %dx%d\n", x, y);
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
  SDL_Rect geometry, g2, g3;
  SDL_Color color = { 255, 255, 255, 128 };  
  KW_Widget * frame, * button, * alabel;
  int i;
  char buf[1024];
  
  /* initialize window and renderer */
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(1280, 768, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 100, 100, 100, 1);
  SDL_RenderSetLogicalSize(renderer, 640, 480);
  TTF_Init();
  
  /* load tileset */

  set = IMG_Load("tileset.png");
  
  /* initialize gui */
  gui = KW_Init(renderer, set);
  font = TTF_OpenFont("SourceSansPro-Semibold.ttf", 12);
  TTF_SetFontHinting(font, TTF_HINTING_NONE);
  KW_SetFont(gui, font);

  geometry.x = 30; geometry.y = 30; geometry.w = 580; geometry.h = 400;
  g2 = geometry;
  g2.x = 10, g2.y = 40, g2.w = 100, g2.h = 30;
  g3.x = 10, g3.y = 10, g3.w = 100, g3.h = 30;
  
  frame = KW_CreateFrame(gui, NULL, &geometry);
  geometry.x = 0; geometry.y = 0;
  frame = KW_CreateScrollbox(gui, frame, &geometry);
  g3.x = 0; g3.y = 0; g3.h = 400; g3.w = 300;
  button = KW_CreateButton(gui, frame, "Opa", &g3);
  KW_AddWidgetDragStartHandler(button, DragStart);
  KW_AddWidgetDragHandler(button, Drag);
  KW_AddWidgetDragStopHandler(button, DragStop);
  
  g2.x = 0, g2.y = 0; g2.w = 640; g2.h = 480;
  /* create another parent frame */
  while (!SDL_QuitRequested()) {
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 100, 100, 200, 1);
    SDL_RenderFillRect(renderer, &g2);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 1);
    KW_Paint(gui);
    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  
  KW_Quit(gui);
  SDL_FreeSurface(set);
  SDL_Quit();
  
  return 0;
}
