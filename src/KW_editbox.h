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

/**
 * \brief   Creates an editbox widget.
 * \details The editbox uses tiles in the third column of the tileset
 * \param   gui The KW_GUI instance that will hold this widget.
 * \param   parent The parent widget of this widget.
 * \param   text The initial text for this editbox
 * \param   geometry The relative geometry of this frame.
 * \return  The editbox instance.
 */
extern DECLSPEC KW_Widget * KW_CreateEditbox(KW_GUI * gui, KW_Widget * parent,
                                             const char * text,
                                             const KW_Rect * geometry);

/**
 * \brief Specifies the editbox text
 * \details Cursor is reset
 * \param widget the editbox widget to change
 * \param text the text to set
 */
extern DECLSPEC void KW_SetEditboxText(KW_Widget * widget, const char * text);

/**
 * \brief Returns the text in the editbox
 * \param widget The editbox instance to return text from
 * \return the text in the editbox
 */
extern DECLSPEC const char * KW_GetEditboxText(KW_Widget * widget);

/**
 * \brief Specify, in terms of characters, the position of the cursor
 * \param widget the edtibox widget to change
 * \param pos the cursor position
 */
extern DECLSPEC void KW_SetEditboxCursorPosition(KW_Widget * widget,
                                                 unsigned int pos);

/**
 * \brief Returns, in terms of characters, where de cursor is
 * \param widget the editbox widget to return the cursor from
 * \return the cursor position
 */
extern DECLSPEC unsigned int KW_GetEditboxCursorPosition(KW_Widget * widget);

/**
 * \brief Changes the current font used to render edtibox text.
 * \param widget The editbox widget to change font
 * \param font the KW_Font instance to use
 */
extern DECLSPEC void KW_SetEditboxFont(KW_Widget * widget, KW_Font * font);

/**
 * \brief Returns the current KW_Font being used in the editbox
 * \param widget the editbox widget to get the font from
 * \return the font being used
 */
extern DECLSPEC KW_Font * KW_GetEditboxFont(KW_Widget * widget);

/**
 * \brief   Gets the associated text color with an editbox.
 * \param   widget The editbox widget.
 * \returns The KW_Color associated with this editbox text.
 * \details Note that if no color was set, it will return the KW_GUI color
 * \sa KW_WasEditboxTextColorSet
 * \sa KW_SetEditboxTextColor
 */
extern DECLSPEC KW_Color KW_GetEditboxTextColor(KW_Widget * widget);

/**
 * \brief   Check if the editbox is using its own color or the default one
 * \param   widget The editbox widget.
 * \returns KW_TRUE if the color was set or KW_FALSE otherwise
 * \sa KW_GetEditboxTextColor
 * \sa KW_SetEditboxTextColor
 */
extern DECLSPEC KW_bool KW_WasEditboxTextColorSet(KW_Widget * widget);

/**
 * \brief   Sets the color of the editbox text.
 * \param   widget The editbox instance.
 * \param   color The color to assign to the editbox.
 */
extern DECLSPEC void KW_SetEditboxTextColor(KW_Widget * widget, KW_Color color);

#ifdef __cplusplus
}
#endif

#endif
