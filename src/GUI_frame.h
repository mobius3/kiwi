#ifndef GUI_FRAME_H
#define GUI_FRAME_H

/**
 * \file GUI_frame.h
 * 
 * Defines functions related to the frame GUI_Widget
 */

#include "GUI_widget.h"

#ifdef __cplusplus
extern "C" {
#endif

struct GUI_GUI;
typedef struct GUI_GUI GUI_GUI;

/**
 * \brief   Creates a frame widget.
 * \details The frame is constructed by arranging the the first 3x3 tiles in the tileset.
 * \param   gui The GUI_GUI instance that will hold this widget.
 * \param   parent The parent widget of this widget.
 * \param   geometry The relative geometry of this frame.
 * \return  The frame instance.
 */
GUI_Widget * GUI_CreateFrame(GUI_GUI * gui, GUI_Widget * parent, const SDL_Rect * geometry);

#ifdef __cplusplus
}
#endif


#endif