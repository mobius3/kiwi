/*
  Copyright (c) 2016, Leonardo Guilherme de Freitas
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

/**
 * \file KW_toggle.h
 * \details A button that has an on and off switch
 */

#ifndef KW_PUSHBUTTON_H
#define KW_PUSHBUTTON_H

#include "KW_gui.h"
#include "KW_widget.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Creates a toggle button that can be checked or not checked.
 * \details The toggle occupies by default the column 12, lines 0 through 12 in the tileset.
 * \param gui The gui to create this widget on
 * \param parent The parentof this widget
 * \param geometry The geometry of this widget. Big toggles may look ugly.
 * \returns The instance of the toggle as a widget
 */
extern DECLSPEC KW_Widget * KW_CreateToggle(KW_GUI * gui, KW_Widget * parent, const KW_Rect * geometry);

/**
 * \brief Returns if the toggle is checked (it was clicked or set) or not
 * \param widget The toggle instance
 * \returns KW_TRUE if it is checked, KW_FALSE otherwise
 */
extern DECLSPEC KW_bool KW_IsToggleChecked(KW_Widget * widget);

/**
 * \brief Set toggle as checked or not
 * \param widget The toggle instance
 * \param checked Pass KW_TRUE to make it checked or KW_FALSE otherwise
 */
extern DECLSPEC void KW_SetToggleChecked(KW_Widget * widget, KW_bool checked);

#ifdef __cplusplus
}
#endif

#endif
