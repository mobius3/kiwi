#include "SDL.h"
#include "KW_gui.h"
#include "KW_frame.h"
#include "KW_label.h"
#include "KW_renderdriver_sdl2.h"

int main(int argc, char ** argv) {
  
  /* initialize window and renderer */
  KW_RenderDriver * driver;
  SDL_Window * window;
  SDL_Renderer * renderer;
  KW_Surface * set;
  KW_GUI * gui;
  KW_Font * font;
  KW_Rect framegeom, labelgeom;
  KW_Widget * frame;
  
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(320, 240, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 200, 100, 100, 1);
  driver = KW_CreateSDL2RenderDriver(renderer, window);
  /* load tileset */
  
  set = KW_LoadSurface(driver, "tileset.png");
  
  /* initialize gui */
  gui = KW_Init(driver, set);
  font = KW_LoadFont(driver, "Fontin-Regular.ttf", 12);
  KW_SetFont(gui, font);
  
  framegeom.x = 10, framegeom.y = 10, framegeom.w = 160, framegeom.h = 120;
  labelgeom = framegeom; labelgeom.x = labelgeom.y = 0;

  /* create 10 frames and 10 labels */
  frame = NULL;
  for (int i = 0; i < 10; i++) {
    frame = KW_CreateFrame(gui, frame, &framegeom);
    KW_CreateLabel(gui, frame, "Yay", &labelgeom);
  }

  while (!SDL_QuitRequested()) {
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
