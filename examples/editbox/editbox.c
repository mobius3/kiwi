/**
 * This is the `editbox` example. It displays an editbox, labels and buttons.
 */

#include "KW_gui.h"
#include "KW_frame.h"
#include "KW_editbox.h"
#include "KW_label.h"
#include "KW_button.h"
#include "KW_renderdriver_sdl2.h"

KW_Widget * editBox;

/* Callback for when the OK button is clicked */
KW_bool quit = KW_FALSE;
void OKClicked(KW_Widget * widget, int b) {
  const char* text = KW_GetEditboxText(editBox);
  printf("%s \n", text);
  quit = KW_TRUE;
}

int main(int argc, char ** argv) {
  /* Initialize SDL */
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window * window;
  SDL_Renderer * renderer;
  KW_Rect windowrect = { 0, 0, 320, 240 };
  SDL_CreateWindowAndRenderer(windowrect.w, windowrect.h, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 100, 100, 200, 1);

  /* Initialize KiWi */
  KW_RenderDriver * driver = KW_CreateSDL2RenderDriver(renderer, window);
  KW_Surface * set = KW_LoadSurface(driver, "tileset.png");
  KW_GUI * gui = KW_Init(driver, set);

  /* Create the top-level framve */
  KW_Rect framerect = { .x = 10, .y = 10, .w = 300, .h = 220 };
  KW_RectCenterInParent(&windowrect, &framerect);
  KW_Widget * frame = KW_CreateFrame(gui, NULL, &framerect);

  /* Create the title, label and edibox widgets */
  KW_Rect titlerect = { .x = 0, .y = 10, .w = 300, .h = 30 };
  KW_Rect labelrect = { .y = 100, .w = 60, .h = 30 };
  KW_Rect editboxrect = { .y = 100, .w = 100, .h = 40 };
  KW_Rect * rects[] = { &labelrect, &editboxrect };
  unsigned weights[] = { 1, 4 };
  KW_RectFillParentHorizontally(&framerect, rects, weights, 2, 10, KW_RECT_ALIGN_MIDDLE);
  KW_CreateLabel(gui, frame, "Editbox example", &titlerect);
  KW_CreateLabel(gui, frame, "Label", &labelrect);
  editBox = KW_CreateEditbox(gui, frame, "Edit me!", &editboxrect);
  KW_Rect buttonrect = { .x = 250, .y = 170, .w = 40, .h = 40 };
  KW_Widget * okbutton = KW_CreateButtonAndLabel(gui, frame, "OK", &buttonrect);
  KW_AddWidgetMouseDownHandler(okbutton, OKClicked);

  /* Main loop */
  while (!SDL_QuitRequested() && !quit) {
    SDL_RenderClear(renderer);
    KW_ProcessEvents(gui);
    KW_Paint(gui);
    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  
  /* free stuff */
  KW_Quit(gui);
  KW_ReleaseSurface(driver, set);
  KW_ReleaseRenderDriver(driver);
  SDL_Quit();
  
  return 0;
}
