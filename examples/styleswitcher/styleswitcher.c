#include "SDL.h"
#include "KW_gui.h"
#include "KW_button.h"
#include "SDL_image.h"

SDL_Texture * setalloy;
SDL_Texture * setfutter;
SDL_Texture * setflat;
SDL_Texture * set;

TTF_Font * fontin;
TTF_Font * sourcepro;
SDL_bool quit = SDL_FALSE;

void kthxbaiClicked(KW_Widget * widget, int b) {
  quit = SDL_TRUE;
}

void SwitchFlatClicked(KW_Widget * widget, int b) {
  KW_GUI * gui = KW_GetGUI(widget);
  SDL_Renderer * renderer = KW_GetRenderer(gui);
  KW_SetFont(gui, sourcepro);
  SDL_SetRenderDrawColor(renderer, 200, 100, 100, 1);  
  KW_SetTileset(gui, setflat);
}

void SwitchAlloyClicked(KW_Widget * widget, int b) {
  KW_GUI * gui = KW_GetGUI(widget);
  SDL_Renderer * renderer = KW_GetRenderer(gui);
  KW_SetFont(gui, sourcepro);
  SDL_SetRenderDrawColor(renderer, 64, 67, 70, 1);  
  KW_SetTileset(gui, setalloy);
}

void SwitchNormalClicked(KW_Widget * widget, int b) {
  KW_GUI * gui = KW_GetGUI(widget);
  SDL_Renderer * renderer = KW_GetRenderer(gui);
  KW_SetFont(gui, fontin);
  SDL_SetRenderDrawColor(renderer, 100, 100, 200, 1);
  KW_SetTileset(KW_GetGUI(widget), set);
}

void SwitchFutterClicked(KW_Widget * widget, int b) {
  KW_GUI * gui = KW_GetGUI(widget);
  SDL_Renderer * renderer = KW_GetRenderer(gui);
  KW_SetFont(gui, fontin);  
  SDL_SetRenderDrawColor(renderer, 118, 152, 162, 1);
  KW_SetTileset(gui, setfutter);
}

int main(int argc, char ** argv) {
  
  /* initialize window and renderer */
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window * window;
  SDL_Renderer * renderer;
  SDL_CreateWindowAndRenderer(320, 240, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 118, 152, 162, 1);
  TTF_Init();
  SDL_Color white = {0,0,0};
  
  /* load tilesets */
  setalloy = IMG_LoadTexture_RW(renderer, SDL_RWFromFile("tileset-alloy.png", "r"), SDL_TRUE);
  setfutter = IMG_LoadTexture_RW(renderer, SDL_RWFromFile("tileset-futterpedia.png", "r"), SDL_TRUE);
  setflat = IMG_LoadTexture_RW(renderer, SDL_RWFromFile("tileset-flat.png", "r"), SDL_TRUE);
  set = IMG_LoadTexture_RW(renderer, SDL_RWFromFile("tileset.png", "r"), SDL_TRUE);

  
  /* initialize gui */
  KW_GUI * gui = KW_Init(renderer, setfutter);
  fontin = TTF_OpenFontRW(SDL_RWFromFile("Fontin-Regular.ttf", "r"), SDL_TRUE, 12);
  sourcepro = TTF_OpenFontRW(SDL_RWFromFile("SourceSansPro-Semibold.ttf", "r"), SDL_TRUE, 12);
  TTF_Font * dejavu = TTF_OpenFontRW(SDL_RWFromFile("DejaVuSans.ttf", "r"), SDL_TRUE, 11);
  
  int h = TTF_HINTING_NONE;
  TTF_SetFontHinting(fontin, h);
  TTF_SetFontHinting(sourcepro, h);
  TTF_SetFontHinting(dejavu, h);
  KW_SetFont(gui, fontin);

  /* top frame */  
  SDL_Rect framegeom, editgeom, labelgeom, buttongeom;
  framegeom.x = 10, framegeom.y = 10, framegeom.w = 300, framegeom.h = 220;
  KW_Widget * frame = KW_CreateFrame(gui, NULL, &framegeom);
  
  /* buttons frame */
  framegeom.x = 10, framegeom.y = 160, framegeom.w = 280, framegeom.h = 48;
  KW_Widget * buttonsframe = KW_CreateFrame(gui, frame, &framegeom);
  
  /* buttons */
  KW_Widget * b = NULL;
  buttongeom.x = 10, buttongeom.y = 8, buttongeom.w = 32, buttongeom.h = 32;
  b = KW_CreateButton(gui, buttonsframe, "", &buttongeom);
  KW_SetWidgetTileset(b, set);
  KW_AddWidgetMouseDownHandler(b, SwitchNormalClicked);
  
  buttongeom.x += 290/4;
  b = KW_CreateButton(gui, buttonsframe, "", &buttongeom);
  KW_SetWidgetTileset(b, setalloy);
  KW_AddWidgetMouseDownHandler(b, SwitchAlloyClicked);
  
  buttongeom.x += 290/4;
  b = KW_CreateButton(gui, buttonsframe, "", &buttongeom);
  KW_SetWidgetTileset(b, setfutter);
  KW_AddWidgetMouseDownHandler(b, SwitchFutterClicked);
  
  buttongeom.x += 290/4;
  b = KW_CreateButton(gui, buttonsframe, "", &buttongeom);
  KW_SetWidgetTileset(b, setflat);
  KW_AddWidgetMouseDownHandler(b, SwitchFlatClicked);
  
  /* reset framegeom */
  framegeom.x = 10, framegeom.y = 10, framegeom.w = 300, framegeom.h = 220;
  framegeom.w -= 20; framegeom.h = 100;
  
  /* create the editbox frames */
  editgeom.x = 120, editgeom.y = 20, editgeom.w = 150, editgeom.h = 35;
  labelgeom.x = 10, labelgeom.y = 20, labelgeom.w = 110, labelgeom.h = 35;
  frame = KW_CreateFrame(gui, frame, &framegeom);
  KW_Widget * editbx = KW_CreateEditbox(gui, frame, "βέβαιος (sure)", &editgeom);
  KW_SetEditboxFont(editbx, dejavu);
  KW_Widget * l = KW_CreateLabel(gui, frame, "Can you do UTF-8?", &labelgeom);
  KW_SetLabelAlignment(l, KW_LABEL_ALIGN_RIGHT, 0, KW_LABEL_ALIGN_MIDDLE, 0);
  
  buttongeom = editgeom;
  buttongeom.y = 60; buttongeom.h -= 10;
  KW_Widget * kthxbai = KW_CreateButton(gui, frame, "kthxbai", &buttongeom);
  KW_AddWidgetMouseDownHandler(kthxbai, kthxbaiClicked);
  
  //SDL_Event ev;
  while (!SDL_QuitRequested() && !quit) {
    //while (SDL_PollEvent(&ev)) KW_PushEvent(gui, &ev);
    SDL_RenderClear(renderer);
    KW_Paint(gui);
    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  
  /* free stuff */
  KW_Quit(gui);
  TTF_CloseFont(fontin);
  TTF_CloseFont(dejavu);
  SDL_DestroyTexture(set);
  TTF_Quit();
  SDL_Quit();
  
  return 0;
}
