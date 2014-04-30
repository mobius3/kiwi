#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H
#include "GUI_widget.h"

/**
 * \file GUI_button.h
 * 
 * Declares functions for creating and managing buttons
 */

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief   Creates a button widget.
 * \details The button widget is a composite widget. It uses the second 3x3 tile block (next to the frame block) to render its lines.
 *          It instantiates a child label to hold the text.
 * \param   gui The GUI_GUI instance that will hold this button.
 * \param   parent The parent widget of this button.
 * \param   text The text to print inside the button.
 * \param   geometry The geometry of this button.
 * \return  The button instance.
 */
GUI_Widget * GUI_CreateButton(GUI_GUI * gui, GUI_Widget * parent, const char * text, const SDL_Rect * geometry);

#ifdef __cplusplus
}
#endif

#endif