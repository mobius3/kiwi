#include "SDL.h"
#include "KW_gui.h"
#include "KW_button.h"
#include "SDL_image.h"

int main(int argc, char ** argv) {
  
  /* initialize window and renderer */
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window * window;
  SDL_Renderer * renderer;
  SDL_CreateWindowAndRenderer(320, 240, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 100, 100, 200, 1);
  SDL_RenderSetLogicalSize(renderer, 320, 240);
  TTF_Init();
  
  /* load tileset */
  SDL_Texture * set;
  set = IMG_LoadTexture_RW(renderer, SDL_RWFromFile("tileset.png", "r"), SDL_TRUE);
  
  /* initialize gui */
  KW_GUI * gui = KW_Init(renderer, set);
  TTF_Font * font = TTF_OpenFontRW(SDL_RWFromFile("Fontin-Regular.ttf", "r"), SDL_TRUE, 12);
  KW_SetFont(gui, font);

  SDL_Rect framegeom, editgeom, labelgeom, buttongeom;
  framegeom.x = 10, framegeom.y = 10, framegeom.w = 300, framegeom.h = 220;

  
  KW_Widget * frame = KW_CreateFrame(gui, NULL, &framegeom);
  buttongeom.x = 120, buttongeom.y = 110, buttongeom.w = 170, buttongeom.h = 30;
  KW_CreateButton(gui, frame, "Friendship? Again?!", &buttongeom);
  
  framegeom.w -= 20; framegeom.h = 100;
  editgeom.x = 120, editgeom.y = 20, editgeom.w = 150, editgeom.h = 30;
  labelgeom.x = 10, labelgeom.y = 20, labelgeom.w = 110, labelgeom.h = 30;
  frame = KW_CreateFrame(gui, frame, &framegeom);
  KW_CreateEditbox(gui, frame, "Editbox #1", &editgeom);
  KW_Widget * l = KW_CreateLabel(gui, frame, "Type your destiny:", &labelgeom);
  KW_SetLabelAlignment(l, KW_LABEL_ALIGN_RIGHT, 0, KW_LABEL_ALIGN_MIDDLE, 0);
  
  editgeom.x = 120, editgeom.y = 50, editgeom.w = 150, editgeom.h = 30;
  labelgeom.x = 10, labelgeom.y = 50, labelgeom.w = 110, labelgeom.h = 30;
  KW_CreateEditbox(gui, frame, "Editbox #2", &editgeom);
  l = KW_CreateLabel(gui, frame, "Again:", &labelgeom);
  KW_SetLabelAlignment(l, KW_LABEL_ALIGN_RIGHT, 0, KW_LABEL_ALIGN_MIDDLE, 0);

 
  while (!SDL_QuitRequested()) {
    SDL_RenderClear(renderer);
    KW_Paint(gui);
    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  
  /* free stuff */
  KW_Quit(gui);
  TTF_CloseFont(font);
  SDL_DestroyTexture(set);
  TTF_Quit();
  SDL_Quit();
  
  return 0;
}
