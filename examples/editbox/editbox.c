#include "SDL.h"
#include "KW_gui.h"
#include "KW_button.h"
#include "KW_frame.h"
#include "KW_editbox.h"
#include "KW_label.h"
#include "KW_renderdriver_sdl2.h"


int main(int argc, char ** argv) {
  SDL_Window * window;
  SDL_Renderer * renderer;
  KW_RenderDriver * driver;
  KW_Surface * set;
  KW_GUI * gui;
  KW_Rect framegeom, editgeom, labelgeom, buttongeom;
  KW_Widget * frame, * editbx, * label;
  KW_Font * fontin, * dejavu;
  
  /* initialize window and renderer, and create a render driver */
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(320, 240, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 100, 100, 200, 1);
  driver = KW_CreateSDL2RenderDriver(renderer, window);

  /* load tileset */
  set = KW_LoadSurface(driver, "tileset.png");
  
  /* initialize gui */
  gui = KW_Init(driver, set);
  fontin = KW_LoadFont(driver, "Fontin-Regular.ttf", 12);
  dejavu = KW_LoadFont(driver, "DejaVuSans.ttf", 12);
  KW_SetFont(gui, fontin);

  framegeom.x = 10, framegeom.y = 10, framegeom.w = 300, framegeom.h = 220;
  frame = KW_CreateFrame(gui, NULL, &framegeom);
  buttongeom.x = 120, buttongeom.y = 110, buttongeom.w = 170, buttongeom.h = 30;
  KW_CreateButton(gui, frame, "Friendship? Again?!", &buttongeom);
  
  framegeom.w -= 20; framegeom.h = 100;
  editgeom.x = 120, editgeom.y = 20, editgeom.w = 150, editgeom.h = 30;
  labelgeom.x = 10, labelgeom.y = 20, labelgeom.w = 110, labelgeom.h = 30;
  frame = KW_CreateFrame(gui, frame, &framegeom);
  editbx = KW_CreateEditbox(gui, frame, "Editbox #1", &editgeom);
  KW_SetEditboxFont(editbx, dejavu);
  label = KW_CreateLabel(gui, frame, "Type your destiny:", &labelgeom);
  KW_SetLabelAlignment(label, KW_LABEL_ALIGN_RIGHT, 0, KW_LABEL_ALIGN_MIDDLE, 0);
  
  editgeom.x = 120, editgeom.y = 50, editgeom.w = 150, editgeom.h = 30;
  labelgeom.x = 10, labelgeom.y = 50, labelgeom.w = 110, labelgeom.h = 30;
  editbx = KW_CreateEditbox(gui, frame, "Editbox #2", &editgeom);
  KW_SetEditboxFont(editbx, dejavu);
  
  label = KW_CreateLabel(gui, frame, "Again:", &labelgeom);
  KW_SetLabelAlignment(label, KW_LABEL_ALIGN_RIGHT, 0, KW_LABEL_ALIGN_MIDDLE, 0);

  while (!SDL_QuitRequested()) {
    SDL_RenderClear(renderer);
    KW_ProcessEvents(gui);
    KW_Paint(gui);
    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  
  /* free stuff */
  KW_Quit(gui);
  KW_ReleaseFont(driver, fontin);
  KW_ReleaseFont(driver, dejavu);
  KW_ReleaseSurface(driver, set);
  KW_ReleaseRenderDriver(driver);
  SDL_Quit();
  
  return 0;
}
