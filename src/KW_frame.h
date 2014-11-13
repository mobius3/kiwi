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

#ifndef KW_FRAME_H
#define KW_FRAME_H

/**
 * \file KW_frame.h
 * 
 * Defines functions related to the frame KW_Widget
 */

#include "KW_widget.h"

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
extern DECLSPEC KW_Widget * KW_CreateFrame(KW_GUI * gui, KW_Widget * parent, const KW_Rect * geometry);

#ifdef __cplusplus
}
#endif


#endif
