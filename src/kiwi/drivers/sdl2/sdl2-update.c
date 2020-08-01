#include "sdl2-update.h"

void KWSDL2_UpdateFunction(KW_Driver * driver, KW_GUI * gui) {
  SDL_Event sdlEvent;
  KW_InputEvent ev;
  SDL_PumpEvents();
  while (SDL_PollEvent(&sdlEvent)) {
    switch (sdlEvent.type) {
      case SDL_MOUSEMOTION:
        ev.type = KW_INPUT_CURSOR_MOTION;
        ev.cursorMotion.cursorId = sdlEvent.motion.which;
        ev.cursorMotion.x = sdlEvent.motion.x;
        ev.cursorMotion.y = sdlEvent.motion.y;
        ev.cursorMotion.xMotion = sdlEvent.motion.xrel;
        ev.cursorMotion.yMotion = sdlEvent.motion.yrel;
        break;
      case SDL_MOUSEBUTTONDOWN:
        ev.type = KW_INPUT_CURSOR_BUTTON_DOWN;
        ev.cursorButtonDown.cursorId = sdlEvent.button.which;
        ev.cursorButtonDown.x = sdlEvent.button.x;
        ev.cursorButtonDown.y = sdlEvent.button.y;
        break;
      case SDL_MOUSEBUTTONUP:
        ev.type = KW_INPUT_CURSOR_BUTTON_UP;
        ev.cursorButtonUp.cursorId = sdlEvent.button.which;
        ev.cursorButtonUp.x = sdlEvent.button.x;
        ev.cursorButtonUp.y = sdlEvent.button.y;
        break;
      case SDL_TEXTINPUT:
        ev.type = KW_INPUT_TEXT;
        SDL_memcpy(ev.text.text, sdlEvent.text.text, 32);
        break;
      case SDL_KEYUP:
        ev.type = KW_INPUT_KEY_UP;
        ev.keyUp.key = sdlEvent.key.keysym.sym;
        break;
      case SDL_KEYDOWN:
        ev.type = KW_INPUT_KEY_DOWN;
        ev.keyDown.key = sdlEvent.key.keysym.sym;
        break;
      default:
        ev.type = KW_INPUT_UNKNOWN;
        break;
    }
    KW_PostGUIInputEvent(gui, &ev);
  }
}
