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

#ifndef KW_GUI_H
#define KW_GUI_H

#include "KW_macros.h"
#include "KW_widget.h"
#include "KW_renderdriver.h"

/** 
 * \file KW_gui.h
 * 
 * Main include for the KiWi library.
 */


/**
 *  \mainpage KiWi
 *
 *  http://www.libsdl.org/
 *
 *  \section intro_sec Introduction
 * 
 *  KiWi is a widget-based GUI library for 2D games using SDL2. Its widgets
 *  are built from a tileset allowing you to customize the looks of your GUI
 *  by simply changing the source tile set.
 * 
 *  You can also create custom widgets.
 * 
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief   The KW_GUI type holds information shared by all widgets.
 * \details You need a KW_GUI instance to create widgets.
 */

/**
 * \brief The type of a font changed handler
 * \sa KW_AddGUITextChangedHandler
 */
typedef void (*KW_OnGUIFontChanged)(KW_GUI * gui, void * data, KW_Font * font);

/**
 * \brief The type of a text color changed handler
 * \sa KW_AddGUITextColorChangedHandler
 */
typedef void (*KW_OnGUITextColorChanged)(KW_GUI * gui, void * data, KW_Color color);


/**
 * \brief   Initializes a new KW_GUI instance.
 * \details Each KW_Widget should be associated with a KW_GUI. For each KW_GUI
 *          that you create, you should call KW_Quit(gui) when quitting. KW_GUI
 *          doesn't take ownership of either the renderer or the tileset.
 * 
 *          The tileset will be available to widgets both in the surface and
 *          texture forms. This is useful if your widget doesn'nt change a lot
 *          and you want to compose a texture to use always.
 * \param   renderer A pointer to a KW_RenderDriver.
 * \param   tileset The tile surface to use as a tileset.
 * \returns A initialized KW_GUI instance.
 */
extern DECLSPEC KW_GUI * KW_Init(KW_RenderDriver * renderer, KW_Surface * tileset);

/**
 * \brief   Quits this KW_GUI instance and free its resources.
 * \details Every pointer passed to any KW_* function will not be free'd
 *          as KW_GUI doesn't take ownership of them, unless stated otherwise.
 *          You still have to free them yourself.
 * \param   gui The KW_GUI instance that is being quitted.
 */
extern DECLSPEC void KW_Quit(KW_GUI * gui);

/**
 * \brief   Sets a new KW_RenderDriver for this KW_GUI instance.
 * \details If you want to render your GUI in another window, you can pass it's
 *          renderer to this function.
 * \param   gui The KW_GUI instance.
 * \param   render The KW_RenderDriver instance.
 */
extern DECLSPEC void KW_SetRenderer(KW_GUI * gui, KW_RenderDriver * renderer);

/**
 * \brief   Returns the current associated renderer with a KW_GUI instance.
 * \details This is useful if you are implementing a custom widget and needs to
 *          render your textures.
 * \param   gui The KW_GUI instance.
 * \return  The associated KW_RenderDriver instance.
 */
extern DECLSPEC KW_RenderDriver * KW_GetRenderer(KW_GUI * gui);

/**
 * \brief   Set a new tileset surface to be used in this KW_GUI instance.
 * \details You may call this function anytime to switch the tileset being used
 *          to render the widgets. A new texture will be created based on this.
 * \param   gui The KW_GUI instance to set a new tileset surface.
 * \param   surface The surface to set. You still have to free the old one.
 */
extern DECLSPEC void KW_SetTilesetSurface(KW_GUI * gui, KW_Surface * tileset);

/**
 * \brief   Gets the tileset texture associated with a KW_GUI instance.
 * \returns A pointer to a const KW_Texture. You shouldn't modify this
 *          texture as it is managed by KiWi.
 * \param   gui The KW_GUI instance to get the tileset from.
 */
extern DECLSPEC KW_Texture * KW_GetTilesetTexture(KW_GUI * gui);

/**
 * \brief   Gets the tileset surface associated with a KW_GUI instance.
 * \returns A pointer to a KW_RenderDriver. You might want to use this to free
 *          the surface.
 * \param   gui The KW_GUI instance to get the tileset from.
 */
extern DECLSPEC KW_Surface * KW_GetTilesetSurface(KW_GUI * gui);

/**
 * \brief   Sets a uniform font to be used in label-rendering widgets.
 * \details All widgets that needs a font to render text will use by default the font set with this function unless
 *          you specify otherwise in their respective SetFont calls.
 * \param   gui The KW_GUI instance to set the font.
 * \param   font The font to associate with a KW_GUI instance.
 */
extern DECLSPEC void KW_SetFont(KW_GUI * gui, KW_Font * font);
/**
 * \brief   Sets a uniform font conlor to be used in label-rendering widgets.
 * \details All widgets that render text will use by default the font color set with this function unless
 *          you specify otherwise in their respective SetLabelColor calls.
 * \param   gui The KW_GUI instance to set the font.
 * \param   color The font color to associate with a KW_GUI instance.
 */
extern DECLSPEC void KW_SetTextColor(KW_GUI * gui, KW_Color color);


/**
 * \brief   Add a function to be called when the current GUI font changes.
 * \details If you are rendering text somewhere, you might want to know when
 *          the font changes to maintain consistency. A new handler will
 *          be added if the pair (handler, priv) wasn't inserted yet.
 * \param   gui The KW_GUI instance that will call your handler.
 * \param   handler The actual handler.
 * \param   priv Private data that will be passed to the handler.
 */
extern DECLSPEC void KW_AddGUIFontChangedHandler(KW_GUI * gui, KW_OnGUIFontChanged handler, void * priv);

/**
 * \brief   Remove a previously added font changed handler.
 * \details The pair (handler, priv) will be removed.
 * \param   gui The KW_GUI instance to remove the handler from.
 * \param   handler The handler to remove.
 * \param   priv The private data to match.
 */
extern DECLSPEC void KW_RemoveGUIFontChangedHandler(KW_GUI * gui, KW_OnGUIFontChanged handler, void * priv);

/**
 * \brief   Add a function to be called when the current GUI text color changes.
 * \details If you are rendering text somewhere, you might want to know when
 *          the font color changes to maintain consistency. A new handler will
 *          be added if the pair (handler, priv) wasn't inserted yet.
 * \param   gui The KW_GUI instance that will call your handler.
 * \param   handler The actual handler.
 * \param   priv Private data that will be passed to the handler.
 */
extern DECLSPEC void KW_AddGUITextColorChangedHandler(KW_GUI * gui, KW_OnGUITextColorChanged handler, void * priv);

/**
 * \brief   Remove a previously added text color changed handler.
 * \details The pair (handler, priv) will be removed.
 * \param   gui The KW_GUI instance to remove the handler from.
 * \param   handler The handler to remove.
 * \param   priv The private data to match.
 */
extern DECLSPEC void KW_RemoveGUITextColorChangedHandler(KW_GUI * gui, KW_OnGUITextColorChanged handler, void * priv);

/**
 * \brief   Get the current font associated with a KW_GUI instance.
 * \details You might want to use this function if you need to render text in a custom widget.
 * \param   gui The KW_GUI instance to get the font from.
 * \return  The current font associated with the KW_GUI instance.
 */
extern DECLSPEC KW_Font * KW_GetFont(KW_GUI * gui);

/**
 * \brief   Get the current font color associated with a KW_GUI instance.
 * \details You might want to use this function if you need to render text in a custom widget.
 * \param   gui The KW_GUI instance to get the font color from.
 * \return  The current font color associated with the KW_GUI instance.
 */
extern DECLSPEC KW_Color KW_GetTextColor(KW_GUI * gui);

/**
 * \brief   Paint all the widgets associated with this GUI.
 * \details This will walk the widget tree, painting all of its widgets, from bottom to top.
 * \param   gui The KW_GUI instance holding the widget tree.
 */
extern DECLSPEC void KW_Paint(KW_GUI * gui);

/**
 * \brief   Process all pending events (mouse movement, keyboard movement, etc)
 * \details This function should be called before KW_Paint. It will update the state of the
 *          widgets based on the position of the cursor, clicks/touches and keyboard inputs.
 *          Most input based callbacks will be called before this function returns.
 * \param   gui The KW_GUI instance holding the widget tree.
 */
extern DECLSPEC void KW_ProcessEvents(KW_GUI * gui);

#ifdef __cplusplus
}
#endif

#endif
