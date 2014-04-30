/*
  Copyright (c) 2014, Leonardo Guilherme de Freitas
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * \file GUI_label.h
 * 
 * Declares functions for creating and managing labels.
 */

#ifndef GUI_LABEL_H
#define GUI_LABEL_H

#include "GUI_widget.h"
#include "SDL_ttf.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name LABEL_STYLE_*
 * 
 * Defines the possible text styles for the label */
#define LABEL_STYLE_NORMAL        0x00
#define LABEL_STYLE_BOLD          0x01
#define LABEL_STYLE_ITALIC        0x02
#define LABEL_STYLE_UNDERLINE     0x04
#define LABEL_STYLE_STRIKETHROUGH 0x08


/**
 * \brief Possible vertial alignments to align a label.
 * \details Text is vertically aligned inside its defined geometry
 */
typedef enum GUI_LabelVerticalAlignment {
  LABEL_ALIGN_TOP,     /// Vertically align label in the top of the geometry.
  LABEL_ALIGN_MIDDLE,  /// Vertically align label in the middle of the geometry.
  LABEL_ALIGN_BOTTOM   /// Vertically align label in the bottom of the geometry.
} GUI_LabelVerticalAlignment;

/**
 * \brief Possible horizontal alignments to align a label
 * \details Text is horizontally aligned inside its defined geometry
 */
typedef enum GUI_LabelHorizontalAlignment {
  LABEL_ALIGN_LEFT,    /// Horizontally align label in the left part of the geometry.
  LABEL_ALIGN_CENTER,  /// Horizontally align label in the center part of the geometry.
  LABEL_ALIGN_RIGHT    /// Horizontally align label in the right part of the geometry.
} GUI_LabelHorizontalAlignment;

/**
 * \brief   Creates a label widget.
 * \details The label uses the font specified in the GUI_GUI instance unless specified otherwise.
 *          The geometry of the label is used to align horizontally and vertically the label and also
 *          to clip its contents if they exceed it.
 * \param   gui The GUI_GUI instance that will hold this widget.
 * \param   parent The parent widget of this widget.
 * \param   geometry The relative geometry of this label.
 * \return  The label instance.
 */
GUI_Widget DECLSPEC * GUI_CreateLabel(GUI_GUI * gui, GUI_Widget * parent, const char * text, const SDL_Rect * geometry);

/**
 * \brief   Set the text being rendered in this label.
 * \param   widget The label widget.
 * \param   text The text to render.
 */
void DECLSPEC GUI_SetLabelText(GUI_Widget * widget, const char * text);

/**
 * \brief   Set the text style for this label.
 * \param   widget The label widget.
 * \param   style The style of the text
 */
void DECLSPEC GUI_SetLabelStyle(GUI_Widget * widget, int style);

/**
 * \brief   Sets the color of the label text.
 * \param   widget The label instance.
 * \param   color The color to assign to the label.
 */
void DECLSPEC GUI_SetLabelColor(GUI_Widget * widget, SDL_Color color);

/**
 * \brief   Changes the font of the label's text.
 * \details Please note that you are responsible for managing the font memory and should free the previous font if its not required anymore.
 * \param   widget The label widget.
 * \param   font The font to assign to the label.
 */
void DECLSPEC GUI_SetLabelFont(GUI_Widget * widget, TTF_Font * font);

/**
 * \brief   Sets label alignment inside a geometry.
 * \param   widget The widget to ajust the alignment.
 * \param   halign The horizontal alignment used.
 * \param   hoffset The horizontal offset to be added to the label position after its aligned.
 * \param   valign The vertical alignment used.
 * \param   voffset The vertical offset to be added to the label position after its aligned.
 */
void DECLSPEC GUI_SetLabelAlignment(GUI_Widget * widget, GUI_LabelHorizontalAlignment halign, int hoffset, GUI_LabelVerticalAlignment valign, int voffset);

/**
 * \brief   Gets the associated font with a label.
 * \param   widget The label widget.
 * \returns A TTF_Font pointer to the currently label's associated font.
 */
TTF_Font DECLSPEC * GUI_GetLabelFont(GUI_Widget * widget);

#ifdef __cplusplus
}
#endif

#endif