KiWi - The Killer Widgets library
=================================

*Killer Widgets* (KiWi) is a widget-based GUI library for game developers to use in their 2D games. Its widgets are constructed by using a tileset, allowing you to easly customize the looks of your GUI by simply changing its tiles (even in runtime, yay!).

It supports custom (and composed) widgets so that you can implement that beautiful ring chart showing how much network traffic your MMORPG is using, allowing you to impress your boss.

KiWi is built around SDL2 and designed to accompany it (means that if you want to use KiWi, you'll have to use SDL. But hey, SDL is awesome already, no big deal, right? :)

Right now KiWi is under (heavy) development and any help (code patches, money, chat, hug, etc) is hugely appreciated.

Here are some screenshots, though (click on them to see code):

[![alt text](https://raw.githubusercontent.com/leonardo2d/KiWi/master/examples/playground/playground-screenshot.png "Playground screenshot")](https://github.com/leonardo2d/KiWi/blob/master/examples/playground/playground.c)

## Build and test KiWi:

If you're feeling mighty and want to try KiWi right now, these are (roughly) the steps to do it:

1. Have a compiler environment ready (GCC, LLVM, MSVC, MinGW, etc);
2. Have [SDL2], [SDL2\_ttf] and [SDL2\_image] available and properly detectable by your compiler;
3. Have [CMake](http://cmake.org) installed;
4. Download this repository;
5. Create a `build` folder inside it;
6. Run `cmake` (or `cmake-gui`), set the binary dir to the newly created build folder and the source dir to the repository folder
7. Build it!

You can test the examples inside the `build/examples/` folder.

[SDL2]: http://libsdl.org
[SDL2_ttf]: https://www.libsdl.org/projects/SDL_ttf/
[SDL2_image]: https://www.libsdl.org/projects/SDL_image/
