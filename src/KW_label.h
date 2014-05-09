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

/**
 * \file KW_label.h
 * 
 * Declares functions for creating and managing labels.
 */

#ifndef KW_LABEL_H
#define KW_LABEL_H

#include "KW_widget.h"
#include "SDL_ttf.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name KW_LABEL_STYLE_*
 * 
 * Defines the possible text styles for the label */
#define KW_LABEL_STYLE_NORMAL        0x00
#define KW_LABEL_STYLE_BOLD          0x01
#define KW_LABEL_STYLE_ITALIC        0x02
#define KW_LABEL_STYLE_UNDERLINE     0x04
#define KW_LABEL_STYLE_STRIKETHROUGH 0x08


/**
 * \brief Possible vertial alignments to align a label.
 * \details Text is vertically aligned inside its defined geometry
 */
typedef enum KW_LabelVerticalAlignment {
  KW_LABEL_ALIGN_TOP,     /// Vertically align label in the top of the geometry.
  KW_LABEL_ALIGN_MIDDLE,  /// Vertically align label in the middle of the geometry.
  KW_LABEL_ALIGN_BOTTOM   /// Vertically align label in the bottom of the geometry.
} KW_LabelVerticalAlignment;

/**
 * \brief Possible horizontal alignments to align a label
 * \details Text is horizontally aligned inside its defined geometry
 */
typedef enum KW_LabelHorizontalAlignment {
  KW_LABEL_ALIGN_LEFT,    /// Horizontally align label in the left part of the geometry.
  KW_LABEL_ALIGN_CENTER,  /// Horizontally align label in the center part of the geometry.
  KW_LABEL_ALIGN_RIGHT    /// Horizontally align label in the right part of the geometry.
} KW_LabelHorizontalAlignment;

/**
 * \brief   Creates a label widget.
 * \details The label uses the font specified in the KW_GUI instance unless specified otherwise.
 *          The geometry of the label is used to align horizontally and vertically the label and also
 *          to clip its contents if they exceed it.
 * \param   gui The KW_GUI instance that will hold this widget.
 * \param   parent The parent widget of this widget.
 * \param   geometry The relative geometry of this label.
 * \return  The label instance.
 */
KW_Widget DECLSPEC * KW_CreateLabel(KW_GUI * gui, KW_Widget * parent, const char * text, const SDL_Rect * geometry);

/**
 * \brief   Set the text being rendered in this label.
 * \param   widget The label widget.
 * \param   text The text to render.
 */
void DECLSPEC KW_SetLabelText(KW_Widget * widget, const char * text);

/**
 * \brief   Set the text style for this label.
 * \param   widget The label widget.
 * \param   style The style of the text
 */
void DECLSPEC KW_SetLabelStyle(KW_Widget * widget, int style);

/**
 * \brief   Sets the color of the label text.
 * \param   widget The label instance.
 * \param   color The color to assign to the label.
 */
void DECLSPEC KW_SetLabelColor(KW_Widget * widget, SDL_Color color);

/**
 * \brief   Changes the font of the label's text.
 * \details Please note that you are responsible for managing the font memory and should free the previous font if its not required anymore.
 * \param   widget The label widget.
 * \param   font The font to assign to the label.
 */
void DECLSPEC KW_SetLabelFont(KW_Widget * widget, TTF_Font * font);

/**
 * \brief   Sets label alignment inside a geometry.
 * \param   widget The widget to ajust the alignment.
 * \param   halign The horizontal alignment used.
 * \param   hoffset The horizontal offset to be added to the label position after its aligned.
 * \param   valign The vertical alignment used.
 * \param   voffset The vertical offset to be added to the label position after its aligned.
 */
void DECLSPEC KW_SetLabelAlignment(KW_Widget * widget, KW_LabelHorizontalAlignment halign, int hoffset, KW_LabelVerticalAlignment valign, int voffset);

/**
 * \brief   Gets the associated font with a label.
 * \param   widget The label widget.
 * \returns A TTF_Font pointer to the currently label's associated font.
 */
TTF_Font DECLSPEC * KW_GetLabelFont(KW_Widget * widget);

#ifdef __cplusplus
}
#endif

#endif
