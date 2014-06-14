#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "KW_gui.h"

int main(int argc, char ** argv) {
  /* init SDL and SDL_ttf */
  SDL_Renderer * renderer;
  SDL_Window * window;
  SDL_Surface * set;
  KW_GUI * gui;
  TTF_Font * font;
  KW_Widget * frame;
  SDL_Rect geometry;
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(320, 240, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 100, 100, 100, 1);
  
  TTF_Init();
  
  /* load tileset surface */
  
  set = IMG_Load("tileset.png");
  
  /* load font */
  font = TTF_OpenFont("Fontin-Regular.ttf", 12);
  
  /* init KiWi */
  gui = KW_Init(renderer, set);
  KW_SetFont(gui, font);
  
  /* create a frame and a label on top of it. */
  geometry.x = geometry.y = 0; geometry.w = 320; geometry.h = 240;
  frame = KW_CreateFrame(gui, NULL, &geometry);
  KW_CreateLabel(gui, frame, "Label", &geometry);
  
  while (!SDL_QuitRequested()) {
    SDL_RenderClear(renderer);
    KW_Paint(gui);
    SDL_Delay(1);
    SDL_RenderPresent(renderer);
  }
  KW_Quit(gui);
  SDL_FreeSurface(set);
  
  return 0;
}
