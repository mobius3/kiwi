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

#include "KW_widget.h"

#ifndef KW_RADIO
#define KW_RADIO

#ifdef __cplusplus
extern "C" {
#endif

struct KW_RadioManager;
typedef struct KW_RadioManager KW_RadioManager;

KW_RadioManager * KW_CreateRadioManager();
void KW_RadioManagerSetChecked(KW_RadioManager * manager, KW_Rect * Rect);
void KW_RadioManagerSetUnchecked(KW_RadioManager * manager, KW_Rect * Rect);

int KW_RadioManagerGetSelected(KW_RadioManager * manager);

extern DECLSPEC KW_Widget * KW_CreateRadio(KW_GUI * gui, KW_Widget * parent,
                                           KW_RadioManager * manager,
                                           const char * text, int value,
                                           const KW_Rect * geometry);

#ifdef __cplusplus
}
#endif

#endif
