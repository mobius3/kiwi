#ifndef KW_SCROLLBOX_H
#define KW_SCROLLBOX_H

#include "KW_gui.h"
#include "KW_widget.h"

/**
 * \brief   Creates a frame that can scroll its children widgets.
 * \details Unless specified otherwise, the scrollbox will scroll its children
 *          both horizonatlly and vertically, contained by its geometry. All
 *          childrens will be clipped to its geometry.
 * 
 *          The scroll box will manage the geometry of each children widget when
 *          its scrolled.
 * \param   geometry The geometry of this scrollbox. Contents will be clipped to
 *          its internal area (inside borders).
 */
extern DECLSPEC KW_Widget * KW_CreateScrollbox(KW_GUI * gui, KW_Widget * parent, const SDL_Rect * geometry);

/**
 * \brief   Scrolls vertically by a given amount in pixels.
 * \param   scrollbox The scrollbox widget to scroll its contents.
 * \param   amount The amount of pixels to vertically scroll.
 */
extern DECLSPEC void KW_ScrollboxVerticalScroll(KW_Widget * scrollbox, int amount);

/**
 * \brief   Scrolls horizontally by a given amount in pixels.
 * \param   scrollbox The scrollbox widget to scroll its contents.
 * \param   amount The amount of pixels to vertically scroll.
 */
extern DECLSPEC void KW_ScrollboxHorizontalScroll(KW_Widget * scrollbox, int amount);

#endif
