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
#include "SDL_ttf.h"

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
 * \param   text The text to print inside the button.
 * \param   geometry The geometry of this button.
 * \return  The button instance.
 */
extern DECLSPEC KW_Widget * KW_CreateButton(KW_GUI * gui, KW_Widget * parent, const char * text, const KW_Rect * geometry);

/**
 * \brief   Sets the text being displayed in a button.
 * \param   text The new text to show.
 */
extern DECLSPEC void KW_SetButtonText(KW_Widget * button, const char * text);

/**
 * \brief   Sets the icon being displayed in a button
 * \param   widget The button widget to show the icon.
 * \param   iconclip The area in the tileset to clip the icon from. Set to NULL
 *          to remove the icon.
 */
extern DECLSPEC void KW_SetButtonIcon(KW_Widget * widget, const KW_Rect * iconclip);

/**
 * \brief   Changes the font of the button's text.
 * \details Please note that you are responsible for managing the font memory and should free the previous font if its not required anymore.
 * \param   widget The button widget.
 * \param   font The font to assign to the button.
 */
extern DECLSPEC void KW_SetButtonFont(KW_Widget * button, TTF_Font * font);

/**
 * \brief   Sets the color of the button text.
 * \param   widget The button instance.
 * \param   color The color to assign to the button.
 */
extern DECLSPEC void KW_SetButtonTextColor(KW_Widget * widget, KW_Color color);

/**
 * \brief   Gets the associated font color with a button.
 * \param   widget The button widget.
 * \returns The KW_Color associated with this button.
 * \details Note that if no color was set, it will return the KW_GUI color
 * \sa KW_WasButtonColorSet
 * \sa KW_SetButtonColor
 */
extern DECLSPEC KW_Color KW_GetButtonTextColor(KW_Widget * widget);

/**
 * \brief   Check if the button is using its own color or the default one
 * \param   widget The button widget.
 * \returns KW_TRUE if the color was set or KW_FALSE otherwise
 * \sa KW_GetButtonColor
 * \sa KW_SetButtonColor
 */
extern DECLSPEC KW_bool KW_WasButtonTextColorSet(KW_Widget * widget);

#ifdef __cplusplus
}
#endif

#endif
