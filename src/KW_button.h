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

#ifndef KW_BUTTON_H
#define KW_BUTTON_H
#include "KW_widget.h"

/**
 * \file KW_button.h
 * 
 * Declares functions for creating and managing buttons
 */

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief   Creates a button widget.
 * \details The button widget is a composite widget. It uses the second 3x3 tile block (next to the frame block) to render its lines.
 * \param   gui The KW_GUI instance that will hold this button.
 * \param   parent The parent widget of this button.
 * \param   label The label widget to print inside the button.
 * \param   geometry The geometry of this button.
 * \return  The button instance.
 */
extern DECLSPEC KW_Widget * KW_CreateButton(KW_GUI * gui, KW_Widget * parent, KW_Widget * label, const KW_Rect * geometry);

/**
 * \brief   Creates a button widget and a label with the text.
 * \details The button widget is a composite widget. It uses the second 3x3 tile block (next to the frame block) to render its lines.
 *          A label will be automatically created with the passed in text.
 * \param   gui The KW_GUI instance that will hold this button.
 * \param   parent The parent widget of this button.
 * \param   text The text to print inside the button.
 * \param   geometry The geometry of this button.
 * \return  The button instance.
 */
extern DECLSPEC KW_Widget * KW_CreateButtonAndLabel(KW_GUI * gui, KW_Widget * parent, const char * text, const KW_Rect * geometry);

/**
 * \brief Sets a new widget as a label
 * \param widget The button widget
 * \param label The new label
 * \returns The old label, reparented to the root widget.
 * \details You might want to destroy the returning widget if you have no use to it.
 */
extern DECLSPEC KW_Widget * KW_SetButtonLabel(KW_Widget * button, KW_Widget * label);

/**
 * \brief Returns the label associated with this widget.
 * \param widget The button widget
 * \returns The widget set as label
 */
extern DECLSPEC KW_Widget * KW_GetButtonLabel(KW_Widget * button);


#ifdef __cplusplus
}
#endif

#endif
