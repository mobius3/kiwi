#include "SDL.h"
#include "KW_gui.h"
#include "KW_button.h"
#include "KW_scrollbox.h"
#include "SDL_image.h"

int dragmode = 0;

void DragStart(KW_Widget * widget, int x, int y) {
  SDL_Rect g;
  KW_GetWidgetAbsoluteGeometry(widget, &g);
  if (x> g.x + g.w - 40 && y > g.y + g.h -40) dragmode = 1;
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
  SDL_Rect geometry = {0, 0, 1280, 768};
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
  SDL_SetRenderDrawColor(renderer, 100, 100, 100, 1);
  TTF_Init();
  
  /* load tileset */

  set = IMG_Load("tileset.png");
  
  /* initialize gui */
  gui = KW_Init(renderer, set);
  font = TTF_OpenFont("SourceSansPro-Semibold.ttf", 12);
  TTF_SetFontHinting(font, TTF_HINTING_NONE);
  KW_SetFont(gui, font);

  geometry.x = geometry.w * 0.0625; geometry.y = geometry.h * .0625; geometry.w *= .875f; geometry.h *= .875;
  frame = KW_CreateScrollbox(gui, NULL, &geometry);
  KW_AddWidgetDragStartHandler(frame, DragStart);
  KW_AddWidgetDragHandler(frame, Drag);
  KW_AddWidgetDragStopHandler(frame, DragStop);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  geometry.x = 0; geometry.y = 0; geometry.h = 150; geometry.w = 150;
  
  for (i = 0; i < 10; i++) {
    geometry.x = i * 40; 
    geometry.y = i * 10;
    button = KW_CreateButton(gui, frame, "Button", &geometry);
    KW_AddWidgetDragStartHandler(button, DragStart);
    KW_AddWidgetDragHandler(button, Drag);
    KW_AddWidgetDragStopHandler(button, DragStop);
  }
    
  /* create another parent frame */
  while (!SDL_QuitRequested()) {
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        geometry.w = ev.window.data1;
        geometry.h = ev.window.data2;
        geometry.x = geometry.w * 0.0625; geometry.y = geometry.h * .0625; geometry.w *= .875f; geometry.h *= .875;
        KW_SetWidgetGeometry(frame, &geometry);
      }
    }
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
