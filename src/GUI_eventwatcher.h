#ifndef GUI_EVENTWATCHER_H
#define GUI_EVENTWATCHER_H

#include "GUI_gui.h"
#include "SDL.h"

#ifdef __cplusplus
extern "C" {
#endif


int GUI_EventWatcher(void * gui, SDL_Event * event);

#ifdef __cplusplus
}
#endif


#endif