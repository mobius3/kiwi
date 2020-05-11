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

#ifndef KW_CHECKBOX_INTERNAL
#define KW_CHECKBOX_INTERNAL

#include "KW_label.h"
#include "KW_label_internal.h"
#include "KW_widget.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct KW_Checkbox {
  /* so it can be used as a label... I think */
  KW_Label label;
  KW_bool  checked;
  KW_Rect  selected;
  KW_Rect  unselected;
} KW_Checkbox;

void DestroyCheckbox(KW_Widget * widget);

#ifdef __cplusplus
}
#endif

#endif
