#ifndef KIWI_WIDGETS_FRAME_H
#define KIWI_WIDGETS_FRAME_H

/**
 * \file KW_frame.h
 * 
 * Defines functions related to the frame KW_Widget
 */

#include "kiwi/core/widget.h"
#include "kiwi/widgets/widgets-export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief   Creates a frame widget.
 * \details The frame is constructed by arranging the the first 3x3 tiles in the tileset.
 * \param   gui The KW_GUI instance that will hold this widget.
 * \param   parent The parent widget of this widget.
 * \param   geometry The relative geometry of this frame.
 * \return  The frame instance.
 */
extern KIWI_WIDGETS_EXPORT KW_Widget * KW_CreateFrame(KW_GUI * gui, KW_Widget * parent, const KW_Rect * geometry);

#ifdef __cplusplus
}
#endif


#endif
