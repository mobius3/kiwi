KiWi - The Killer Widgets library
=================================

*Killer Widgets* (KiWi) is a widget-based GUI library for game developers to use in their 2D games. Its widgets are constructed by using a tileset, allowing you to easly customize the looks of your GUI by simply changing its tiles (even in runtime, yay!).

It supports custom (and composed) widgets so that you can implement that beautiful ring chart showing how much network traffic your MMORPG is using, allowing you to impress your boss.

KiWi is built around SDL2 and designed to accompany it (means that if you want to use KiWi, you'll have to use SDL. But hey, SDL is awesome already, no big deal, right? :)

Right now KiWi is under (heavy) development and any help (code patches, money, chat, hug, etc) is hugely appreciated.

Here are some screenshots, though (click on them to see code):

[![alt text](https://raw.githubusercontent.com/leonardo2d/KiWi/master/examples/playground/playground-screenshot.png "Playground screenshot")](https://github.com/leonardo2d/KiWi/blob/master/examples/playground/playground.c)
[![alt text](https://raw.githubusercontent.com/leonardo2d/KiWi/master/examples/frame-family/frame-family-screenshot.png "A Family of Frames")](https://github.com/leonardo2d/KiWi/blob/master/examples/frame-family/frame-family.c)

[![alt text](https://raw.githubusercontent.com/leonardo2d/KiWi/master/examples/editbox/editbox-screenshot.png "Two Editboxes and a button")](https://github.com/leonardo2d/KiWi/blob/master/examples/editbox/editbox.c)

Check the [roadmap]!

## Build and test KiWi:

If you're feeling mighty and want to try KiWi right now, these are (roughly) the steps to do it:

1. Have a compiler environment ready (GCC, LLVM, MSVC, MinGW, etc);
2. Have [SDL2], [SDL2_ttf] and [SDL2_image] available and properly detectable by your compiler;
3. Have [CMake](http://cmake.org) installed;
4. Download this repository;
5. Create a `build` folder inside it;
6. Run `cmake` (or `cmake-gui`), set the binary dir to the newly created build folder and the source dir to the repository folder;
7. Build it!

You can test the examples inside the `build/examples/` folder.

## Basic usage

Here is a very basic code that draws a label on screen:

```cpp
#include "SDL.h"
#include "SDL_ttf.h"
#include "KW_gui.h"

int main(int argc, char ** argv) {
  /* init SDL and SDL_ttf */
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Renderer * renderer;
  SDL_Window * window;
  SDL_CreateWindowAndRenderer(320, 240, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 200, 100, 100, 1);
  TTF_Init();
  
  /* load tileset */
  SDL_Texture * set;
  set = IMG_LoadTexture(renderer, "tileset.png");
  
  /* load font */
  TTF_Font * font = TTF_OpenFont("Fontin-Regular.ttf", 12);
  
  /* init KiWi */
  KW_GUI * gui = KW_Init(renderer, set);
  KW_SetFont(gui, font);
  
  /* create a frame and a label on top of it. */
  SDL_Rect g = {0, 0, 320, 240};
  KW_Widget * frame = KW_CreateFrame(gui, NULL, &g);
  KW_CreateLabel(gui, frame, "Label", &g);
  
  while (!SDL_QuitRequested()) {
    KW_Paint(gui);
    SDL_Delay(1);
  }
  
  return 0;
}
```

Better documentation is yet to come, but every public method is well documented.

[KW_CreateWidget]: https://github.com/leonardo2d/KiWi/blob/master/src/KW_widget.h#L106
[SDL2]: http://libsdl.org
[SDL2_ttf]: https://www.libsdl.org/projects/SDL_ttf/
[SDL2_image]: https://www.libsdl.org/projects/SDL_image/
[roadmap]: https://github.com/leonardo2d/KiWi/blob/master/ROADMAP.md
