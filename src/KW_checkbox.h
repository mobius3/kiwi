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

#ifndef KW_CHECKBOX
#define KW_CHECKBOX

#include "KW_widget.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * \brief   Creates a checkbox widget.
 * \details The checkbox uses the font specified in the KW_GUI instance unless specified otherwise.
 *          The geometry of the label is used to align horizontally and
 * vertically the label and also to clip its contents if they exceed it.
 * \param   gui The KW_GUI instance that will hold this widget.
 * \param   parent The parent widget of this widget.
 * \param   geometry The relative geometry of this label.
 * \return  The checkbox instance.
 * \sa KW_CreateLabel
 */
extern DECLSPEC KW_Widget * KW_CreateCheckbox(KW_GUI * gui, KW_Widget * parent,
                                              const char *    text,
                                              const KW_Rect * geometry);

/**
 * \brief   Gets The Checked Status of the Widget.
 * \param   widget The label widget.
 */
KW_bool KW_IsCheckboxChecked(KW_Widget * widget);

void KW_CheckboxSetChecked(KW_Widget * widget, KW_Rect * Rect);
void KW_CheckboxSetUnchecked(KW_Widget * widget, KW_Rect * Rect);

#ifdef __cplusplus
}
#endif

#endif
