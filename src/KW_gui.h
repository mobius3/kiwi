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

#ifndef KW_GUI_H
#define KW_GUI_H

#include "SDL.h"
#include "SDL_ttf.h"

#include "KW_macros.h"
#include "KW_widget.h"
#include "KW_frame.h"
#include "KW_label.h"
#include "KW_editbox.h"


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
 *  KiWi is based on SDL2 and SDL2_ttf, requiring these to be present.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief   The KW_GUI type holds information shared by all widgets.
 * \details You need a KW_GUI instance to create widgets.
 */
struct KW_GUI;
typedef struct KW_GUI KW_GUI;

struct KW_Widget;
typedef struct KW_Widget KW_Widget;

/**
 * \brief   Initializes a new KW_GUI instance.
 * \details Each KW_Widget should be associated with a KW_GUI. For each KW_GUI that you create,
 *          you should call KW_Quit(gui) when quitting. KW_GUI doesn't take ownership of either
 *          the renderer or the tileset.
 * \param   renderer A pointer to a SDL_Renderer.
 * \param   tileset A pointer to a SDL_Texture to be used as GUI tileset.
 * \returns A initialized KW_GUI instance.
 */
extern DECLSPEC KW_GUI * KW_Init(SDL_Renderer * renderer, SDL_Texture * tileset);

/**
 * \brief   Quits this KW_GUI instance and free its resources.
 * \details Every pointer passed to any KW_* function will not be free'd
 *          as KW_GUI doesn't take ownership of them, unless stated otherwise.
 *          You still have to free them yourself.
 * \param   gui The KW_GUI instance that is being quitted.
 */
extern DECLSPEC void KW_Quit(KW_GUI * gui);

/**
 * \brief   Sets a new SDL_Renderer for this KW_GUI instance.
 * \details If you want to render your GUI in another window, you can pass it's renderer to this function.
 * \param   gui The KW_GUI instance.
 * \param   render The SDL_Renderer instance.
 */
extern DECLSPEC void KW_SetRenderer(KW_GUI * gui, SDL_Renderer * renderer);

/**
 * \brief   Returns the current associated renderer with a KW_GUI instance.
 * \details This is useful if you are implementing a custom widget and needs to render your textures.
 * \param   gui The KW_GUI instance.
 * \return  The associated SDL_Renderer instance.
 */
extern DECLSPEC SDL_Renderer * KW_GetRenderer(KW_GUI * gui);

/**
 * \brief   Set a new tileset texture to be used in this KW_GUI instance.
 * \details You may call this function anytime to switch the tileset being used to render the widgets.
 * \param   gui The KW_GUI instance to set a new tileset texture.
 * \param   texture The texture to set. You still have to free the old one.
 */
extern DECLSPEC void KW_SetTileset(KW_GUI * gui, SDL_Texture * tileset);

/**
 * \brief   Gets the tileset texture associated with a KW_GUI instance.
 * \details This is useful if you have custom tiles or images in your tileset. You also may need to call this
 *          to free the old tileset before using KW_SetTileset.
 * \param   gui The KW_GUI instance to get the tileset from.
 */
extern DECLSPEC SDL_Texture * KW_GetTileset(KW_GUI * gui);

/**
 * \brief   Sets a uniform font to be used in label-rendering widgets.
 * \details All widgets that needs a font to render text will use by default the font set with this function unless
 *          you specify otherwise in their respective SetFont calls.
 * \param   gui The KW_GUI instance to set the font.
 * \param   font The font to associate with a KW_GUI instance.
 */
extern DECLSPEC void KW_SetFont(KW_GUI * gui, TTF_Font * font);

/**
 * \brief   Get the current font associated with a KW_GUI instance.
 * \details You might want to use this function if you need to render text in a custom widget.
 * \param   gui The KW_GUI instance to get the font from.
 * \return  The current font associated with the KW_GUI instance.
 */
extern DECLSPEC TTF_Font * KW_GetFont(KW_GUI * gui);

/**
 * \brief   Paint all the widgets associated with this GUI.
 * \details This will walk the widget tree, painting all of its widgets, from bottom to top.
 * \param   gui The KW_GUI instance holding the widget tree.
 */
extern DECLSPEC void KW_Paint(KW_GUI * gui);

/**
 * \brief   Adds a new event to be processed by KiWi
 * \details Events needs to be pushed in the same thread that renders because
 *          some SDL calls are dangerous to call from other threads (looking
 *          at you, SDL_StartTextInput()!) on some devices (looking at you,
 *          Android!)
 * \param   gui The KW_GUI instance that will process the events.
 * \param   ev The event to be processed.
 */
extern DECLSPEC void KW_PushEvent(KW_GUI * gui, SDL_Event * ev);

#ifdef __cplusplus
}
#endif

#endif
