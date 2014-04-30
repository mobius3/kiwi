#ifndef KW_EVENTWATCHER_H
#define KW_EVENTWATCHER_H

#include "KW_gui.h"
#include "SDL.h"

#ifdef __cplusplus
extern "C" {
#endif


int KW_EventWatcher(void * gui, SDL_Event * event);

#ifdef __cplusplus
}
#endif


#endif