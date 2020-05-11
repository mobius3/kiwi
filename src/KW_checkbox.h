#ifndef KW_CHECKBOX_H
#define KW_CHECKBOX_H

/**
 * \file KW_checkbox.h
 * A checkbox that is a composition of a toggle widget and another widget to serve as label.
 * Checkboxes can use any widget as a label (it might be another complex widget tree, if you need).
 */

#include "KW_widget.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Creates a new checkbox.
 * \details Checkboxes listen to the mouseup event, when they happen either in the label
 *          or in the toggle, the checkbox value will be toggled.
*  \param gui The KW_GUI instance
 * \param parent The parent widget
 * \param label The label widget. The label will be reparented to be a child of this widget.
 * \param geometry The geometry of this widget
 * \returns The instance of the checkbox
 */
extern DECLSPEC KW_Widget * KW_CreateCheckbox(KW_GUI * gui, KW_Widget * parent, KW_Widget * label, const KW_Rect * geometry);

/**
 * \brief Returns the label associated with this widget.
 * \param widget The checkbox widget
 * \returns The widget set as label
 */
extern DECLSPEC KW_Widget * KW_GetCheckboxLabel(KW_Widget * widget);

/**
 * \brief Sets a new widget as a label
 * \param widget The checkbox widget
 * \param label The new label
 * \returns The old label, reparented to the root widget.
 * \details You might want to destroy this widget if you have no use to it.
 */
extern DECLSPEC KW_Widget * KW_SetCheckboxLabel(KW_Widget * widget, KW_Widget * label);

#ifdef __cplusplus
}
#endif

#endif
