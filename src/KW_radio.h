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

#include "KW_widget.h"

#ifndef KW_RADIO
#define KW_RADIO

#ifdef __cplusplus
extern "C" {
#endif

/**
 * KW_RadioManager type. You need one manager for every set of radio buttons (of
 * which only one of these may be selected at a time)
 */
struct KW_RadioManager;
typedef struct KW_RadioManager KW_RadioManager;

/**
 * allocates a Radio object, will need to be freed at the end of use (after all
 * managed radio buttons are destroyed)
 */
extern DECLSPEC KW_RadioManager * KW_CreateRadioManager();

/**
 * \brief Sets the checked icon to be used
 * \param manager the manager to be used
 * \param Rect the icon sprite
 */
extern DECLSPEC void KW_RadioManagerSetCheckedIcon(KW_RadioManager * manager,
                                                   KW_Rect *         Rect);

/**
 * \brief Sets the unchecked icon to be used
 * \param manager the manager to be used
 * \param Rect the icon sprite
 */
extern DECLSPEC void KW_RadioManagerSetUncheckedIcon(KW_RadioManager * manager,
                                                     KW_Rect *         Rect);

/**
 * \brief Gets the int/enum of the selected element
 * \param manager the set of radiobuttons to get the selected value
 * \returns the value of the selected value, 0 otherwise
 */
extern DECLSPEC int KW_RadioManagerGetSelected(KW_RadioManager * manager);

/**
 * \brief   Creates a radio widget.
 * \details Create Radio widget, very similar to the KW_CreateLabel function, but there are two added arguments
 * \param   gui The KW_GUI instance that will hold this widget.
 * \param   parent The parent widget of this widget.
 * \param manager the radio manager the radiobox needs to be added to
 * \param value the enum value that will be returned by KW_RadioManagerGetSelected if it is selected
 * \param   geometry The relative geometry of this label.
 * \return  The radiobutton instance.
 * \sa KW_CreateLabel
 */
extern DECLSPEC KW_Widget * KW_CreateRadio(KW_GUI * gui, KW_Widget * parent,
                                           KW_RadioManager * manager,
                                           const char * text, int value,
                                           const KW_Rect * geometry);

/**
 * \return the userdata stored in the KW_RadioManager*
 */
extern DECLSPEC void * KW_GetRadioManagerUserData(KW_RadioManager * manager);

/**
 * \brief sets the userdata stored in the KW_RadioManager*
 */
extern DECLSPEC void KW_SetRadioManagerUserData(KW_RadioManager * manager,
                                                void *            data);
#ifdef __cplusplus
}
#endif

#endif
