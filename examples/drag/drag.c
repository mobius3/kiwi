#include <stdio.h>
#include "SDL.h"
#include "KW_gui.h"
#include "KW_button.h"
#include "SDL_image.h"
#include "KW_frame.h"
#include "KW_renderdriver_sdl2.h"

int dragmode = 0;

void DragStart(KW_Widget * widget, int x, int y) {
  KW_Rect g;
  KW_GetWidgetAbsoluteGeometry(widget, &g);
  if (x > g.x + g.w - 20 && y > g.y + g.h - 20) dragmode = 1;
  else dragmode = 0;
  printf("Drag has started at %dx%d\n", x, y);
}

void DragStop(KW_Widget * widget, int x, int y) {
  printf("Drag has stopped at %dx%d\n", x, y);
}

void Drag(KW_Widget * widget, int x, int y, int xrel, int yrel) {
  KW_Rect g;
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
  KW_RenderDriver * driver;
  KW_Surface * set;
  KW_GUI * gui;
  KW_Font * font;
  KW_Rect framegeom;
  KW_Widget * frame, *a;
  
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 200, 100, 100, 1);
  driver = KW_CreateSDL2RenderDriver(renderer, window);
  set = KW_LoadSurface(driver, "tileset.png");

  /* initialize gui */
  gui = KW_Init(driver, set);
  font = KW_LoadFont(driver, "Fontin-Regular.ttf", 12);
  KW_SetFont(gui, font);
  frame = NULL;
  framegeom.x = 50, framegeom.y = 50, framegeom.w = 100, framegeom.h = 100;
  frame = KW_CreateFrame(gui, frame, &framegeom);
  framegeom.x = 10, framegeom.y = 10, framegeom.w = 160, framegeom.h = 120;
  KW_AddWidgetDragHandler(frame, Drag);
  for (int i = 0; i < 2; i++) {
    framegeom.x = framegeom.y = i * 10;
    a = KW_CreateButtonAndLabel(gui, frame, "Yay", &framegeom);
    KW_AddWidgetDragStartHandler(a, DragStart);
    KW_AddWidgetDragStopHandler(a, DragStop);
    KW_AddWidgetDragHandler(a, Drag);
  }
  
  while (!SDL_QuitRequested()) {
    SDL_SetRenderDrawColor(renderer, 200, 100, 100, 1);
    SDL_RenderClear(renderer);
    KW_ProcessEvents(gui);
    KW_Paint(gui);
    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  
  /* free stuff */
  KW_Quit(gui);
  KW_ReleaseFont(driver, font);
  KW_ReleaseSurface(driver, set);
  KW_ReleaseRenderDriver(driver);
  SDL_Quit();
  
  return 0;
}
