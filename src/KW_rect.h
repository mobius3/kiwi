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

#ifndef KW_RECT_H
#define KW_RECT_H

#include "KW_macros.h"

/**
 * The KW_Rect struct represents a rectangle.
 */
typedef struct KW_Rect {
  int x;
  int y;
  int w;
  int h;
} KW_Rect;

/**
 * \brief Possible vertical alignments to align rectangles.
 */
typedef enum KW_RectVerticalAlignment {
  KW_RECT_ALIGN_VERTICALLY_NONE,    /** Do not align vertically. */
  KW_RECT_ALIGN_TOP,     /** Top align the rectangles */
  KW_RECT_ALIGN_MIDDLE,  /** Make the rectangles centralized on a horizontal lign  */
  KW_RECT_ALIGN_BOTTOM   /** Bottom align the rectangles */
} KW_RectVerticalAlignment;

/**
 * \brief Possible horizontal alignments to align rectangles
 */
typedef enum KW_RectHorizontalAlignment {
  KW_RECT_ALIGN_HORIONTALLY_NONE,  /* Do not align horizontally */
  KW_RECT_ALIGN_LEFT,    /** Left align the rectangles */
  KW_RECT_ALIGN_CENTER,  /** Make the rectangles centralized on a vertical lign */
  KW_RECT_ALIGN_RIGHT    /** Right align the rectangles */
} KW_RectHorizontalAlignment;


/**
 * Macro to test if a rectangle is empty
 */
#define KW_IsRectEmpty(r) \
  (!(r).x && !(r).y && !(r).w && !(r).h)

/**
 * \brief Center a rectangle horizontally and vertically inside another.
 * \details This function changes the inner rectangle x and y coordinates so that it is
 *          centered inside the outer rectangle.
 * \param outer The outer rectangle
 * \param inner The inner rectangle to center inside outer.
 */
extern DECLSPEC void KW_RectCenterInParent(KW_Rect * outer, KW_Rect * inner);

/**
 * \brief Vertically center a rectangle inside another
 * \details This function changes inner rectangle y coordinate so that it is
 *          vertically centered inside the outer rectangle.
 * \param outer The outer rectangle
 * \param inner The inner rectangle to center inside outer.
 */
extern DECLSPEC void KW_RectVerticallyCenterInParent(KW_Rect * outer, KW_Rect * inner);

/**
 * \brief Horizontally center a rectangle inside another
 * \details This function changes inner rectangle x coordinate so that it is
 *          horizontally centered inside the outer rectangle.
 * \param outer The outer rectangle
 * \param inner The inner rectangle to center inside outer.
 */
extern DECLSPEC void KW_RectHorizontallyCenterInParent(KW_Rect * outer, KW_Rect * inner);

/**
 * \brief Layout rectangles vertically (like a list), aligning them horizontally.
 * \param rects The array of rectangles to layout
 * \param count The amount of rectangles in the array
 * \param padding Amount of padding between the rectangles
 * \param align How to align the rectangles
 */
extern DECLSPEC void KW_RectLayoutVertically(KW_Rect * rects[], unsigned count, int padding, KW_RectHorizontalAlignment align);

/**
 * \brief Layout rectangles horizontally, aligning them vertically.
 * \param rects The array of rectangles to layout
 * \param count The amount of rectangles in the array
 * \param padding Amount of padding between the rectangles
 * \param align How to align the rectangles
 */
extern DECLSPEC void KW_RectLayoutHorizontally(KW_Rect * rects[], unsigned count, int padding, KW_RectVerticalAlignment align);

/**
 * \brief Layout and resize rectangles vertically, adjusting their height so they all fit inside the outer rectangle.
 * * \details This does not ajust the item width nor align them.
 * \param rects The array of rectangles to layout
 * \param weight The weight of each rectangle passed in @p rects.
 *               The higher the weight, the more space the rectangle will occupy.
 * \param count The amount of rectangles in the array
 * \param padding The amount of padding between the rectangles
 */
extern DECLSPEC void KW_RectFillParentVertically(KW_Rect * outer, KW_Rect * rects[], unsigned weights[], unsigned count, int padding);

/**
 * \brief Layout and resize rectangles horizontally, adjusting their width so they all fit inside the outer
 * rectangle.
 * \details This does not ajust the item height nor align them.
 * \param rects The array of rectangles to layout
 * \param weight The weight of each rectangle passed in @p rects.
 *               The higher the weight, the more space the rectangle will occupy.
 * \param count The amount of rectangles in the array
 * \param padding The amount of padding between the rectangles
 */
extern DECLSPEC void KW_RectFillParentHorizontally(KW_Rect * outer, KW_Rect * rects[], unsigned weights[], unsigned count,
                                             int padding, KW_RectVerticalAlignment align);


/**
 * \brief Calculates an enclosing rectangle that fits all rectangles
 * \details This function finds out the smallest x and y and the biggest w and h
 * \param rects The rectangles array
 * \param count The amount of rectangles in the array
 * \param outer The outer rectangle to be filled
 */
extern DECLSPEC void KW_RectCalculateEnclosingRect(KW_Rect * rects[], unsigned count, KW_Rect * outer);
#endif
