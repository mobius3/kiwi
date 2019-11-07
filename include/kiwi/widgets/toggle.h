/**
 * \file KW_toggle.h
 * \details A button that has an on and off switch
 */

#ifndef KIWI_WIDGETS_TOGGLE_H
#define KIWI_WIDGETS_TOGGLE_H

#include "kiwi/core/gui.h"
#include "kiwi/core/widget.h"
#include "kiwi/widgets/widgets-export.h"

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
extern KIWI_WIDGETS_EXPORT KW_Widget * KW_CreateToggle(KW_GUI * gui, KW_Widget * parent, const KW_Rect * geometry);

/**
 * \brief Returns if the toggle is checked (it was clicked or set) or not
 * \param widget The toggle instance
 * \returns KW_TRUE if it is checked, KW_FALSE otherwise
 */
extern KIWI_WIDGETS_EXPORT KW_bool KW_IsToggleChecked(KW_Widget * widget);

/**
 * \brief Set toggle as checked or not
 * \param widget The toggle instance
 * \param checked Pass KW_TRUE to make it checked or KW_FALSE otherwise
 */
extern KIWI_WIDGETS_EXPORT void KW_SetToggleChecked(KW_Widget * widget, KW_bool checked);

#ifdef __cplusplus
}
#endif

#endif
