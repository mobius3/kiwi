#include "KW_gui.h"
#include "KW_frame.h"
#include "KW_label.h"
#include "KW_renderdriver_sdl2.h"

int main(int argc, char ** argv) {
  /* init SDL and SDL_ttf */
  KW_RenderDriver * driver;
  SDL_Renderer * renderer;
  SDL_Window * window;
  KW_Surface * set;
  KW_GUI * gui;
  KW_Font * font;
  KW_Widget * frame, * l;
  KW_Rect geometry, c;
  
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(320, 240, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 100, 100, 100, 1);
  driver = KW_CreateSDL2RenderDriver(renderer, window);
  
  /* load tileset surface */
  set = KW_LoadSurface(driver, "tileset.png");
  
  /* load font */
  font = KW_LoadFont(driver, "Fontin-Regular.ttf", 12);
  
  /* init KiWi */
  gui = KW_Init(driver, set);
  KW_SetFont(gui, font);
  /* create a frame and a label on top of it. */
  geometry.x = geometry.y = 0; geometry.w = 320; geometry.h = 240;
  frame = KW_CreateFrame(gui, NULL, &geometry);
  l = KW_CreateLabel(gui, frame, "Label with an icon :)", &geometry);
  c.x = 0; c.y = 48; c.w = 24; c.h = 24;
  KW_SetLabelIcon(l, &c);

  while (!SDL_QuitRequested()) {
    SDL_RenderClear(renderer);
    KW_Paint(gui);
    SDL_Delay(1);
    SDL_RenderPresent(renderer);
  }
  KW_Quit(gui);
  KW_ReleaseSurface(driver, set);
  
  return 0;
}
