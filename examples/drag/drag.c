#include "SDL.h"
#include "KW_gui.h"
#include "KW_button.h"
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
  
  /* initialize window and renderer */
  SDL_Window * window;
  SDL_Renderer * renderer;
  SDL_Surface * set;
  KW_GUI * gui;
  TTF_Font * font;
  SDL_Rect framegeom;
  KW_Widget * frame, *a;
  int i = 0;  
  
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 200, 100, 100, 1);
  TTF_Init();
  
  /* load tileset */
  set = IMG_Load("tileset.png");
  
  /* initialize gui */
  gui = KW_Init(renderer, set);
  font = TTF_OpenFont("Fontin-Regular.ttf", 12);
  KW_SetFont(gui, font);
  frame = NULL;
  framegeom.x = 10, framegeom.y = 10, framegeom.w = 300, framegeom.h = 220;  
  framegeom.x = 50, framegeom.y = 50, framegeom.w = 100, framegeom.h = 100;
  frame = KW_CreateFrame(gui, frame, &framegeom);
  framegeom.x = 10, framegeom.y = 10, framegeom.w = 160, framegeom.h = 120;
  KW_AddWidgetDragHandler(frame, Drag);
  for (i = 0; i < 2; i++) {
    framegeom.x = framegeom.y = i * 10;
    a = KW_CreateButton(gui, frame, "Yay", &framegeom);
    KW_AddWidgetDragStartHandler(a, DragStart);
    KW_AddWidgetDragStopHandler(a, DragStop);
    KW_AddWidgetDragHandler(a, Drag);
  }
  
  while (!SDL_QuitRequested()) {
    SDL_SetRenderDrawColor(renderer, 200, 100, 100, 1);
    SDL_RenderClear(renderer);
    KW_Paint(gui);
    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  
  /* free stuff */
  KW_Quit(gui);
  TTF_CloseFont(font);
  SDL_FreeSurface(set);
  TTF_Quit();
  SDL_Quit();
  
  return 0;
}
