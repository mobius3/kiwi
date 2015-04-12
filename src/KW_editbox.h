/*
  Copyright (c) 2014, Leonardo Guilherme de Freitas
  All rights reserved.

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

     1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.

     2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.

     3. This notice may not be removed or altered from any source
     distribution.
*/

#ifndef KW_EDITBOX_H
#define KW_EDITBOX_H

/**
 * \file KW_editbox.h
 * 
 * Defines functions related to creating and managing edit boxes
 */

#include "KW_widget.h"
#include "SDL_ttf.h"

#ifdef __cplusplus
extern "C" {
#endif

extern DECLSPEC KW_Widget * KW_CreateEditbox(KW_GUI * gui, KW_Widget * parent,
                                             const char * text,
                                             const SDL_Rect * geometry);

extern DECLSPEC void KW_SetEditboxText(KW_Widget * widget, const char * text);
extern DECLSPEC const char * KW_GetEditboxText(KW_Widget * widget);
extern DECLSPEC void KW_SetEditboxCursorPosition(KW_Widget * widget,
                                                 unsigned int pos);
extern DECLSPEC unsigned int KW_GetEditboxCursorPosition(KW_Widget * widget);
extern DECLSPEC void KW_SetEditboxFont(KW_Widget * widget, TTF_Font * font);
extern DECLSPEC TTF_Font * KW_GetEditboxFont(KW_Widget * widget);

#ifdef __cplusplus
}
#endif

#endif
