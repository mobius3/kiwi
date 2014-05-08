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
  SDL_SetRenderDrawColor(renderer, 100, 100, 100, 1);
  
  TTF_Init();
  
  /* load tileset */
  SDL_Texture * set;
  set = IMG_LoadTexture(renderer, "tileset.png");
  
  /* initialize gui */
  KW_GUI * gui = KW_Init(renderer, set);
  TTF_Font * font = TTF_OpenFont("Fontin-Regular.ttf", 12);
  KW_SetFont(gui, font);

  SDL_Rect geometry, g2, g3;
  SDL_Color color = { 255, 255, 255, 128 };

  geometry.x = 30; geometry.y = 30; geometry.w = 200; geometry.h = 200;
  g2 = geometry;
  g2.x = 10, g2.y = 40, g2.w = 100, g2.h = 30;
  g3.x = 10, g3.y = 10, g3.w = 100, g3.h = 30;
  
  KW_Widget * frame = KW_CreateFrame(gui, NULL, &geometry);
  KW_Widget * button = KW_CreateButton(gui, frame, "Button", &g3);
  g3.y+=g2.h;
  KW_CreateButton(gui, frame, "Button", &g3);
  g3.y+=g2.h;
  KW_CreateButton(gui, frame, "Button", &g3);
  g3.y+=g2.h;
  KW_CreateButton(gui, frame, "Button", &g3);
  g3.y+=g2.h;
  KW_CreateButton(gui, frame, "Button", &g3);
  
  g2.x = 0, g2.y = -5, g2.w = 200, g2.h = 20;
  KW_Widget * alabel = KW_CreateLabel(gui, frame, "Chose your destiny.", &g2);
  KW_SetLabelAlignment(alabel, KW_LABEL_ALIGN_CENTER, 0, KW_LABEL_ALIGN_TOP, 0);
  KW_SetLabelColor(alabel, color);

  //KW_SetLabelStyle(alabel, LABEL_STYLE_BOLD);
  
  /* create another parent frame */
  SDL_Event ev;
  while (!SDL_QuitRequested()) {
    
    while (SDL_PollEvent(&ev)) {
      switch (ev.type) {
        case SDL_KEYDOWN:
          switch (ev.key.keysym.sym) {
            case SDLK_UP:
              geometry.h -= 10;
              break;
            case SDLK_DOWN:
              geometry.h += 10;
              break;
            case SDLK_LEFT:
              geometry.w -= 10;
              break;
            case SDLK_RIGHT:
              geometry.w += 10;
              break;
              
           case SDLK_w:
             geometry.y -= 10;
             break;
             
           case SDLK_s:
             geometry.y += 10;
             break;
             
           case SDLK_d:
             geometry.x += 10;
             break;
             
           case SDLK_a:
             geometry.x -= 10;
             break;
          }
          printf("Got keysym %d\n", ev.key.keysym.scancode);
          
          KW_SetWidgetGeometry(frame, &geometry);
      }
    }
    SDL_RenderClear(renderer);
    KW_Paint(gui);
    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  
  KW_Quit(gui);
  SDL_Quit();
  
  return 0;
}
