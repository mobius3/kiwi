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

#ifdef __cplusplus
extern "C" {
#endif

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
  KW_RECT_ALIGN_HORIZONTALLY_NONE,  /* Do not align horizontally */
  KW_RECT_ALIGN_LEFT,    /** Left align the rectangles */
  KW_RECT_ALIGN_CENTER,  /** Make the rectangles centralized on a vertical lign */
  KW_RECT_ALIGN_RIGHT    /** Right align the rectangles */
} KW_RectHorizontalAlignment;

/**
 * \brief Center a rectangle horizontally and vertically inside another.
 * \details This function changes the inner rectangle x and y coordinates so that it is
 *          centered inside the outer rectangle.
 * \param outer The outer rectangle
 * \param inner The inner rectangle to center inside outer.
 */
extern DECLSPEC void KW_RectCenterInParent(const KW_Rect * outer, KW_Rect * inner);

/**
 * \brief Vertically center a rectangle inside another
 * \details This function changes inner rectangle y coordinate so that it is
 *          vertically centered inside the outer rectangle.
 * \param outer The outer rectangle
 * \param inner The inner rectangle to center inside outer.
 */
extern DECLSPEC void KW_RectVerticallyCenterInParent(const KW_Rect * outer, KW_Rect * inner);

/**
 * \brief Horizontally center a rectangle inside another
 * \details This function changes inner rectangle x coordinate so that it is
 *          horizontally centered inside the outer rectangle.
 * \param outer The outer rectangle
 * \param inner The inner rectangle to center inside outer.
 */
extern DECLSPEC void KW_RectHorizontallyCenterInParent(const KW_Rect * outer, KW_Rect * inner);

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
extern DECLSPEC void KW_RectFillParentVertically(const KW_Rect * outer, KW_Rect * rects[], unsigned weights[], unsigned count, int padding);

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
extern DECLSPEC void KW_RectFillParentHorizontally(const KW_Rect * outer, KW_Rect * rects[], unsigned weights[], unsigned count,
                                             int padding, KW_RectVerticalAlignment align);


/**
 * \brief Calculates an enclosing rectangle that fits all rectangles
 * \details This function finds out the smallest x and y and the biggest w and h
 * \param rects The rectangles array
 * \param count The amount of rectangles in the array
 * \param outer The outer rectangle to be filled
 */
extern DECLSPEC void KW_RectCalculateEnclosingRect(KW_Rect * const rects[], unsigned count, KW_Rect * outer);

/**
 * \brief Sets the x, y, w, and h of an rect
 * \param rect The target rect
 * \param x Value for x
 * \param y Value for y
 * \param w Value for w
 * \param h Value for h
 */
extern DECLSPEC void KW_SetRect(KW_Rect * rect, int x, int y, int w, int h);

/**
 * \brief Copies an rect into another
 * \param src The rect that will be copied from
 * \param dst The rect that will be copied to
 */
extern DECLSPEC void KW_CopyRect(const KW_Rect * src, KW_Rect * dst);

/**
 * \brief Zeroes an rectangle, making all its values zero.
 * \param rect The rect to zero.
 */
extern DECLSPEC void KW_ZeroRect(KW_Rect * rect);

/**
 * \brief Uses @p margin to resize @p inner so that its poisition and size margins the @p outer rect.
 * \param outer The outer rect
 * \param inner The inner rect that will be resized
 * \param margin The margin to set.
 */
extern DECLSPEC void KW_MarginRect(const KW_Rect * outer, KW_Rect * inner, int margin);

#define KW_PRINTRECT(command, rect) { \
  command("%dx%d+%dx%d\n", (rect).x, (rect).y, (rect).w, (rect).h); \
}

#ifdef __cplusplus
}
#endif

#endif
