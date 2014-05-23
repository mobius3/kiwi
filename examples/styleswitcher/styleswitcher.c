#include "SDL.h"
#include "KW_gui.h"
#include "KW_button.h"
#include "SDL_image.h"

SDL_Surface * setalloy;
SDL_Surface * setfutter;
SDL_Surface * setflat;
SDL_Surface * set;

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
  KW_SetTilesetSurface(gui, setflat);
}

void SwitchAlloyClicked(KW_Widget * widget, int b) {
  KW_GUI * gui = KW_GetGUI(widget);
  SDL_Renderer * renderer = KW_GetRenderer(gui);
  KW_SetFont(gui, sourcepro);
  SDL_SetRenderDrawColor(renderer, 64, 67, 70, 1);  
  KW_SetTilesetSurface(gui, setalloy);
}

void SwitchNormalClicked(KW_Widget * widget, int b) {
  KW_GUI * gui = KW_GetGUI(widget);
  SDL_Renderer * renderer = KW_GetRenderer(gui);
  KW_SetFont(gui, fontin);
  SDL_SetRenderDrawColor(renderer, 100, 100, 200, 1);
  KW_SetTilesetSurface(gui, set);
}

void SwitchFutterClicked(KW_Widget * widget, int b) {
  KW_GUI * gui = KW_GetGUI(widget);
  SDL_Renderer * renderer = KW_GetRenderer(gui);
  KW_SetFont(gui, fontin);  
  SDL_SetRenderDrawColor(renderer, 118, 152, 162, 1);
  KW_SetTilesetSurface(gui, setfutter);
}

int main(int argc, char ** argv) {
  SDL_Window * window;
  SDL_Renderer * renderer;
  KW_GUI * gui;
  TTF_Font * dejavu;
  SDL_Rect framegeom, editgeom, labelgeom, buttongeom;
  KW_Widget * editbx, * label, * button, * kthxbai, * frame, * buttonsframe;
  int hinting;
  
  /* initialize window and renderer */
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(320, 240, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 118, 152, 162, 1);
  TTF_Init();
  
  /* load tilesets */
  setalloy = IMG_Load_RW(SDL_RWFromFile("tileset-alloy.png", "r"), SDL_TRUE);
  setfutter = IMG_Load_RW(SDL_RWFromFile("tileset-futterpedia.png", "r"), SDL_TRUE);
  setflat = IMG_Load_RW(SDL_RWFromFile("tileset-flat.png", "r"), SDL_TRUE);
  set = IMG_Load_RW(SDL_RWFromFile("tileset.png", "r"), SDL_TRUE);

  
  /* initialize gui */
  gui = KW_Init(renderer, setfutter);
  fontin = TTF_OpenFontRW(SDL_RWFromFile("Fontin-Regular.ttf", "r"), SDL_TRUE, 12);
  sourcepro = TTF_OpenFontRW(SDL_RWFromFile("SourceSansPro-Semibold.ttf", "r"), SDL_TRUE, 12);
  dejavu = TTF_OpenFontRW(SDL_RWFromFile("DejaVuSans.ttf", "r"), SDL_TRUE, 11);
  
  hinting = TTF_HINTING_NONE;
  TTF_SetFontHinting(fontin, hinting);
  TTF_SetFontHinting(sourcepro, hinting);
  TTF_SetFontHinting(dejavu, hinting);
  KW_SetFont(gui, fontin);

  /* top frame */  
  framegeom.x = 10, framegeom.y = 10, framegeom.w = 300, framegeom.h = 220;
  frame = KW_CreateFrame(gui, NULL, &framegeom);
  
  /* buttons frame */
  framegeom.x = 10, framegeom.y = 160, framegeom.w = 280, framegeom.h = 48;
  buttonsframe = KW_CreateFrame(gui, frame, &framegeom);
  
  /* buttons */
  button = NULL;
  buttongeom.x = 10, buttongeom.y = 8, buttongeom.w = 32, buttongeom.h = 32;
  button = KW_CreateButton(gui, buttonsframe, "", &buttongeom);
  KW_SetWidgetTilesetSurface(button, set);
  KW_AddWidgetMouseDownHandler(button, SwitchNormalClicked);
  
  buttongeom.x += 290/4;
  button = KW_CreateButton(gui, buttonsframe, "", &buttongeom);
  KW_SetWidgetTilesetSurface(button, setalloy);
  KW_AddWidgetMouseDownHandler(button, SwitchAlloyClicked);
  
  buttongeom.x += 290/4;
  button = KW_CreateButton(gui, buttonsframe, "", &buttongeom);
  KW_SetWidgetTilesetSurface(button, setfutter);
  KW_AddWidgetMouseDownHandler(button, SwitchFutterClicked);
  
  buttongeom.x += 290/4;
  button = KW_CreateButton(gui, buttonsframe, "", &buttongeom);
  KW_SetWidgetTilesetSurface(button, setflat);
  KW_AddWidgetMouseDownHandler(button, SwitchFlatClicked);
  
  /* reset framegeom */
  framegeom.x = 10, framegeom.y = 10, framegeom.w = 300, framegeom.h = 220;
  framegeom.w -= 20; framegeom.h = 100;
  
  /* create the editbox frames */
  editgeom.x = 120, editgeom.y = 20, editgeom.w = 150, editgeom.h = 35;
  labelgeom.x = 10, labelgeom.y = 20, labelgeom.w = 110, labelgeom.h = 35;
  frame = KW_CreateFrame(gui, frame, &framegeom);
  editbx = KW_CreateEditbox(gui, frame, "βέβαιος (sure)", &editgeom);
  KW_SetEditboxFont(editbx, dejavu);
  label = KW_CreateLabel(gui, frame, "Can you do UTF-8?", &labelgeom);
  KW_SetLabelAlignment(label, KW_LABEL_ALIGN_RIGHT, 0, KW_LABEL_ALIGN_MIDDLE, 0);
  
  buttongeom = editgeom;
  buttongeom.y = 60; buttongeom.h -= 10;
  kthxbai = KW_CreateButton(gui, frame, "kthxbai", &buttongeom);
  KW_AddWidgetMouseDownHandler(kthxbai, kthxbaiClicked);
  
  while (!SDL_QuitRequested() && !quit) {
    SDL_RenderClear(renderer);
    KW_Paint(gui);
    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  
  /* free stuff */
  KW_Quit(gui);
  TTF_CloseFont(fontin);
  TTF_CloseFont(dejavu);
  SDL_FreeSurface(set);
  SDL_FreeSurface(setalloy);
  SDL_FreeSurface(setflat);
  SDL_FreeSurface(setfutter);
  TTF_Quit();
  SDL_Quit();
  
  return 0;
}
