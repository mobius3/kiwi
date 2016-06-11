#include "SDL.h"
#include "KW_gui.h"
#include "KW_button.h"
#include "KW_scrollbox.h"
#include "KW_renderdriver_sdl2.h"

int dragmode = 0;

void DragStart(KW_Widget * widget, int x, int y) {
  KW_Rect g;
  KW_GetWidgetAbsoluteGeometry(widget, &g);
  if (x> g.x + g.w - 40 && y > g.y + g.h -40) dragmode = 1;
  else dragmode = 0;
}

void DragStop(KW_Widget * widget, int x, int y) {
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
  SDL_Window * window;
  SDL_Renderer * renderer;
  KW_RenderDriver * driver;
  KW_Surface * set;
  KW_GUI * gui;
  KW_Font * font;
  KW_Rect geometry = {0, 0, 320, 240};
  KW_Widget * frame, * button;
  int i;
  SDL_Event ev;
  
  /* initialize SDL */
  SDL_Init(SDL_INIT_EVERYTHING);

  
#if defined(ANDROID)
  /* enjoy all the screen size on android */
  i = SDL_GetNumVideoDisplays();
  if (i < 1) exit(1);
  SDL_GetDisplayBounds(0, &geometry);
#endif
  SDL_CreateWindowAndRenderer(geometry.w, geometry.h, SDL_WINDOW_RESIZABLE, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 100, 200, 100, 1);
  driver = KW_CreateSDL2RenderDriver(renderer, window);
  set = KW_LoadSurface(driver, "tileset.png");
  
  /* initialize gui */
  gui = KW_Init(driver, set);
  font = KW_LoadFont(driver, "SourceSansPro-Semibold.ttf", 12);
  KW_SetFont(gui, font);

  geometry.x = (unsigned)(geometry.w * 0.0625f);
  geometry.y = (unsigned)(geometry.h * .0625f);
  geometry.w *= .875f;
  geometry.h *= .875;
  frame = KW_CreateScrollbox(gui, NULL, &geometry);
  KW_AddWidgetDragStartHandler(frame, DragStart);
  KW_AddWidgetDragHandler(frame, Drag);
  KW_AddWidgetDragStopHandler(frame, DragStop);

  geometry.x = 10; geometry.y = 0; geometry.h = 40; geometry.w = 230;
  
  for (i = 0; i < 5; i++) {
    button = KW_CreateButtonAndLabel(gui, frame, "Drag me, resize me.", &geometry);
    KW_AddWidgetDragStartHandler(button, DragStart);
    KW_AddWidgetDragHandler(button, Drag);
    KW_AddWidgetDragStopHandler(button, DragStop);
    geometry.y += geometry.h;
  }
    
  /* create another parent frame */
  while (!SDL_QuitRequested()) {
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        geometry.w = (unsigned)ev.window.data1;
        geometry.h = (unsigned)ev.window.data2;
        geometry.x = (unsigned)(geometry.w * 0.0625);
        geometry.y = (unsigned)(geometry.h * .0625);
        geometry.w *= .875f;
        geometry.h *= .875;
        KW_SetWidgetGeometry(frame, &geometry);
      }
    }
    SDL_RenderClear(renderer);
    KW_ProcessEvents(gui);
    KW_Paint(gui);
    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  KW_ReleaseFont(driver, font);
  KW_Quit(gui);
  KW_ReleaseSurface(driver, set);
  SDL_Quit();
  
  return 0;
}
