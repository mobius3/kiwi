/**
 * This is the `editbox` example. It displays an editbox, labels and buttons.
 */

#include "KW_gui.h"
#include "KW_frame.h"
#include "KW_editbox.h"
#include "KW_label.h"
#include "KW_radio.h"
#include "KW_checkbox.h"
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
  KW_Rect windowrect = { 0, 0, 500, 500 };
  SDL_CreateWindowAndRenderer(windowrect.w, windowrect.h, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 100, 100, 200, 1);

  /* Initialize KiWi */
  KW_RenderDriver * driver = KW_CreateSDL2RenderDriver(renderer, window);
  KW_Surface * set = KW_LoadSurface(driver, "tileset.png");
  KW_GUI * gui = KW_Init(driver, set);

  KW_Rect r = {x : 25, y : 25, w : 450, h : 50};
  KW_CreateLabel(gui, NULL, "Checkbox/Radiobox Demo", &r);
  r.y+=50;

    r.w = 100;
    KW_Widget  *checkbox;
    KW_Rect checked = { .x = 96, .y = 24, .w = 24, .h = 24 };
    KW_Rect unchecked = { .x = 96, .y = 0, .w = 24, .h = 24 };
    for(int i =0;i<8;i++){
      if(i%3==0 && i >0){r.x+=150;r.y-=150;}
      r.y+=50;
      checkbox = KW_CreateCheckbox(gui, NULL, "Checkbox", &r);
      KW_CheckboxSetChecked(checkbox, &checked);
      KW_CheckboxSetUnchecked(checkbox, &unchecked);
    }

    r.x-=300;
    r.y+=50;

    KW_Rect rchecked = { .x = 72, .y = 0, .w = 24, .h = 24 };
    KW_RadioManager* m = KW_CreateRadioManager();
    KW_RadioManagerSetChecked(m, &rchecked);
    KW_RadioManagerSetUnchecked(m, &unchecked);

    for(int i =0;i<4;i++){
      r.y+=50;
      checkbox = KW_CreateRadio(gui, NULL, m, "Radio", i+1, &r);
    }



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
