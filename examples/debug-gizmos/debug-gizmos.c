#include "SDL.h"
#include "KW_gui.h"
#include "KW_button.h"
#include "KW_scrollbox.h"
#include "KW_renderdriver_sdl2.h"

KW_Widget * frame;
static KW_Rect buttongeom = {0, 0, 230, 40};

void AddButtonClicked(KW_Widget * button, int mbutton) {
  (void)mbutton;
  char buf[256];
  SDL_snprintf(buf, 255, "geometry: %dx%d+%dx%d", buttongeom.x, buttongeom.y, buttongeom.w, buttongeom.h);
  KW_Widget * w = KW_CreateButtonAndLabel(KW_GetGUI(button), frame, buf, &buttongeom);
  KW_EnableWidgetDebug(w, KW_TRUE);
  buttongeom.y -= buttongeom.h;
}

int main(int argc, char ** argv) {
  SDL_Window * window;
  SDL_Renderer * renderer;
  KW_RenderDriver * driver;
  KW_Surface * set;
  KW_GUI * gui;
  KW_Font * font;
  KW_Rect geometry = {0, 0, 320, 240};
  SDL_Event ev;
  KW_Rect addbuttongeom = {320/2 - 100/2, 240 - 40, 100, 40};

  /* initialize SDL */
  SDL_Init(SDL_INIT_EVERYTHING);
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
  geometry.h -= 30; geometry.h *= .875;
  buttongeom.y = geometry.h;
  frame = KW_CreateScrollbox(gui, NULL, &geometry);
  KW_EnableWidgetDebug(frame, KW_TRUE);

  KW_Widget * addbutton = KW_CreateButtonAndLabel(gui, NULL, "Add button", &addbuttongeom);
  KW_AddWidgetMouseDownHandler(addbutton, AddButtonClicked);
  geometry.x = 10; geometry.y = 0; geometry.h = 40; geometry.w = 230;

  while (!SDL_QuitRequested()) {
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        geometry.w = (unsigned)ev.window.data1;
        geometry.h = (unsigned)ev.window.data2;
        geometry.x = (unsigned)(geometry.w * 0.0625);
        geometry.y = (unsigned)(geometry.h * .0625);
        geometry.w *= .875f;
        geometry.h -= 30; geometry.h *= .875;
        addbuttongeom.x = (unsigned)ev.window.data1/2 - addbuttongeom.w/2;
        addbuttongeom.y = (unsigned)ev.window.data2 - addbuttongeom.h;
        KW_SetWidgetGeometry(frame, &geometry);
        KW_SetWidgetGeometry(addbutton, &addbuttongeom);
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
