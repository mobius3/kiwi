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

#ifndef KW_WIDGET_H
#define KW_WIDGET_H

#include "KW_macros.h"
#include "SDL.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief   The KW_Widget type represents a widget on the KW_GUI instance.
 * \details Regardless of the widget type, they are all represented by this opaque type.
 */
struct KW_Widget;
typedef struct KW_Widget KW_Widget;


struct KW_GUI;
typedef struct KW_GUI KW_GUI;

/* forward declarations of these */
extern DECLSPEC SDL_Renderer * KW_GetRenderer(KW_GUI * gui);
extern DECLSPEC KW_GUI * KW_GetGUI(KW_Widget * widget);



typedef void (*KW_WidgetPaintFunction)(KW_Widget * widget);
typedef void (*KW_WidgetDestroyFunction)(KW_Widget * widget);

/* mouse callbacks */
typedef void (*KW_OnMouseOver)(KW_Widget * widget);
typedef void (*KW_OnMouseLeave)(KW_Widget * widget);
typedef void (*KW_OnMouseDown)(KW_Widget * widget, int button);
typedef void (*KW_OnMouseUp)(KW_Widget * widget, int button);

/* focus callbacks */
typedef void (*KW_OnFocusGain)(KW_Widget * widget);
typedef void (*KW_OnFocusLose)(KW_Widget * widget);

/**
 * \brief   The KW_WidgetType enumeration represents available widget types.
 * \details Every widget created must set a widget type even if its a custom widget.
 */
typedef enum KW_WidgetType {
  KW_WIDGETTYPE_NONE,
  KW_WIDGETTYPE_FRAME,
  KW_WIDGETTYPE_LABEL,
  KW_WIDGETTYPE_BUTTON,
  KW_WIDGETTYPE_EDITBOX,
  KW_WIDGETTYPE_CUSTOM0 = 0x100000,
  KW_WIDGETTYPE_CUSTOM1,
  KW_WIDGETTYPE_CUSTOM2,
  KW_WIDGETTYPE_CUSTOM3,
  KW_WIDGETTYPE_CUSTOM4,
  KW_WIDGETTYPE_CUSTOM5,
  KW_WIDGETTYPE_CUSTOM6,
  KW_WIDGETTYPE_CUSTOM7,
  KW_WIDGETTYPE_CUSTOM8,
  KW_WIDGETTYPE_CUSTOM9,
  KW_WIDGETTYPE_CUSTOM10,
  KW_WIDGETTYPE_CUSTOM11,
  KW_WIDGETTYPE_CUSTOM12,
  KW_WIDGETTYPE_CUSTOM13,
  KW_WIDGETTYPE_CUSTOM14,
  KW_WIDGETTYPE_CUSTOM15,
  KW_WIDGETTYPE_CUSTOM16,
  KW_WIDGETTYPE_CUSTOM17,
  KW_WIDGETTYPE_CUSTOM18,
  KW_WIDGETTYPE_CUSTOM19,
  KW_WIDGETTYPE_CUSTOM20
} KW_WidgetType;


/**
 * \brief   Creates a new custom widget instance.
 * \details If you want to provide a new custom widget this is the function that you should call.
 *          All KW_Widget instances are built using this class, if you should need additional widget 
 *          information you can use the data parameter.
 *          
 *          The widgetpaint function is called to paint the widget after the parent widget has been painted, never
 *          before, so you are garanteed to be rendering on top of it.
 *         
 *          Widget geometry is relative to its parent. Consider the following figure:
 * 
 * \verbatim
 * 
 * 10x10.---------------------------.
 *    |         Root Widget         |
 *    |                             |
 *    |                             |
 *  10x30.------------.             |
 *    |  |Root's Child|             |
 *    |  |            |             |    
 *    |  |  40x20.---------------.  |
 *    |  |       | Child's Child |  |
 *    |  |       |               |  |
 *    |  |       |               |  |
 *    |  |       `---------------´80x40
 *    |  |            |             |
 *    |  `------------´70x90        |
 *    |                             |
 *    `-----------------------------´150x150
 * 
 * Geometry is represented as XxY+WxH.
 * 
 * \endverbatim
 *  
 *          The root widget starts at 10x10. The child widget starts at 10x30 *relative* to the root widget. That means that
 *          its absolute position is actually 20x40! The 'Child's Child' widget starts at 40x20 relative to its parent but its
 *          absolute position is (10 + 10 + 40, 10 + 30 + 20) = 60x60. If the parent widget moves, all its children move together.
 *  
 *          Widgets also have a composed geometry that is calculated by considering each child geometry. They represent the total
 *          area occupied by a widget and its children. In the above picture,
 *          the 'Root's Child' widget composed geometry is calculated by taking each of its children composed geometry and checking
 *          if they are out of the current widget composed geometry. Since 'Child's Child' width and height is out of 'Root's Child'
 *          geometry, 'Root's Child' composed geometry ends up not being 10x30+70x90, but rather 10x30+(40+80+70x90) = 10x30+120x90.
 *          Note the width was updated but not the height.
 * 
 *          The absolute geometry of a widget is calculating by iterating the parent tree and adding their position (x and y).
 * 
 * \param   gui The KW_GUI instace that will hold this widget in its widget tree.
 * \param   parent The parent widget.
 * \param   type The type of this widget.
 * \param   geometry The geometry of this widget *relative* to its parent.
 * \param   widgetpaing The paint function of this widget.
 * \param   widgetdestroy The destroy function of this widget.
 * \param   data Any private data associated with this widget.
 * \returns The new widget.
 */
extern DECLSPEC KW_Widget * KW_CreateWidget(KW_GUI * gui, 
                                              KW_Widget * parent, 
                                              KW_WidgetType type, 
                                              const SDL_Rect * geometry, 
                                              KW_WidgetPaintFunction widgetpaint,
                                              KW_WidgetDestroyFunction widgetdestroy,
                                              void * data);


/**
 * \brief   Change the parent of a widget to another parent.
 * \details Remove \p widget as a child from its current parent and adds it as a child to \p parent.
 *          Every child of \p widget will then belong to the new \p parent tree. Note that \p widget
 *          geometry is **NOT** updated to keep it in the previous absolute position. You should call
 *          KW_SetWidgetGeometry() to update it.
 * 
 *          Keep in mind that this is one of the costliest functions to call in the KiWi library as it
 *          might involve memory reallocation both in the previous and in the new parent widget. The composed
 *          geometry also needs to be updated in both parents in a chain that might go as high as the root widget.
 *          You should avoid widget reparenting if you can.
 * 
 *          To make the widget a top-level widget you should pass \c NULL as the @p parent parameter.
 * 
 * \param   widget The widget that will have its parent changed.
 * \param   parent The new \p widget parent.
 */
extern DECLSPEC void KW_ReparentWidget(KW_Widget * widget, KW_Widget * parent);

/**
 * \brief   Retrieves the parent widget of a widget.
 * \param   widget The KW_Widget instance to retrieve the parent from.
 * \returns The parent widget or NULL if its a top-level widget.
 */
extern DECLSPEC KW_Widget * KW_GetWidgetParent(KW_Widget * widget);

/**
 * \brief   Retrieves the current data associated with a widget.
 * \details If you are implementing a custom widget, you must call this function to access the data you
 *          set up previously when the widget was created.
 * \param   widget The widget to retrieve the data from.
 * \returns The data pointer.
 */
extern DECLSPEC void * KW_GetWidgetData(KW_Widget * widget);

/**
 * \brief   Destroys a widget and free its resources.
 * \details Any custom resource that you set (fonts, custom textures, etc) needs to be freed by you unless specified otherwise.
 * \param   widget The widget to destroy.
 * \param   destroychildren Set to 1 if you want also to destroy all childrens (and their childrens) of this widget. If not, they will be
 *                          reparented to @p widget parent.
 */
extern DECLSPEC void KW_DestroyWidget(KW_Widget * widget, int destroychildren);

/**
 * \brief   Returns the associated KW_GUI instance of a widget.
 * \details If you have a widget instance, this is the means of retrieving its KW_GUI instance.
 * \param   widget The widget to get the KW_GUI instance from.
 * \return  The KW_GUI instance associated with the widget.
 */
extern DECLSPEC KW_GUI * KW_GetWidgetGUI(KW_Widget * widget);

/**
 * \brief   Returns the SDL_Renderer associated with a KW_GUI instance that the @p widget is associated with.
 * \details This is a helper function and its effects are the same of `KW_GetRenderer(KW_GetWidgetGUI(widget))`
 * \param   widget The widget to get the SDL_Renderer instance from.
 * \returns The associated SDL_Renderer instance.
 */
extern DECLSPEC SDL_Renderer * KW_GetWidgetRenderer(KW_Widget * widget);

/**
 * \brief   Sets a new geometry for a widget.
 * \details Use this function if you need to change the widget relative position or its size.
 * \param   widget The widget that will have its size changed.
 * \param   geometry The new widget geometry.
 */
extern DECLSPEC void KW_SetWidgetGeometry(KW_Widget * widget, const SDL_Rect * geometry);

/**
 * \brief   Gets the widget geometry.
 * \details Use this function to retrieve the current *relative* geometry of the widget.
 * \param   widget The widget to get the geometry from.
 * \param   geometry A SDL_Rect pointer that will receive the @p widget geometry. 
 */
extern DECLSPEC void KW_GetWidgetGeometry(KW_Widget * widget, SDL_Rect * geometry);

/**
 * \brief   Gets the absolute widget geometry.
 * \details Use this if you need to know the absolute widget geometry calculated taking its parents in consideration.
 *          It is useful inside the paint function of a widget.
 * \param   widget The widget to get the absolute geometry from.
 * \param   geometry A SDL_Rect pointer that will receive the @p widget absolute geometry.
 */
extern DECLSPEC void KW_GetWidgetAbsoluteGeometry(KW_Widget * widget, SDL_Rect * geometry);

/**
 * \brief   Gets the composed geometry of a widget.
 * \details   Returns the total area occupied by a widget and its children. Note that the position is
 *          still relative to its parent widget.
 * \param   widget The widget to get the compsoed geometry from.
 * \param   geometry A SDL_Rect pointer that will receive the @p widget composed geometry.
 */
extern DECLSPEC void KW_GetWidgetComposedGeometry(KW_Widget * widget, SDL_Rect * composed);

/**
 * \brief   Paint a widget and all of its children unless they are hidden.
 * \details Iterates through all of the @p widget subtree calling their paint functions.
 *          If you wish to paint all widgets of a KW_GUI instance it is better to call
 *          `KW_Paint()` rather than this function.
 * \param   widget The widget to paint.
 */
extern DECLSPEC void KW_PaintWidget(KW_Widget * widget);

/* Stuff now related to handling of events (focus, mouse, keyboard, etc) */

/**
 * \brief   Block this widget from receiving input events (mouse, keyboard, touch, etc)
 * \details You can use this function if you want to keep the widget from receiving input related events.
 *          The widget will not know if the cursor is over it, if a key was pressed, etc.
 *         
 *          If you have composite widgets and want the widgets on top to not steal mouse-over or press events
 *          from the widgets below this function might be useful to you.
 * 
 *          All children widgets will also have its input evenets blocked.
 * \param   widget The widget that will stop receiving input evenets.
 */
extern DECLSPEC void KW_BlockWidgetInputEvents(KW_Widget * widget);

/**
 * \brief   Unblocks this widget from receiving input events.
 * \details See ::KW_BlockWidgetEvents. All children widgets will have its input evenets unblocked.
 * \param   widget The widget that will now receive input events.
 */
extern DECLSPEC void KW_UnblockWidgetInputEvents(KW_Widget * widget);

/**
 * \brief   Adds a function that will be called whenever the mouse/cursor gets over the widget.
 * \details If you want to know when the cursor is over the widget, you can add a handler to the list.
 * \param   widget The widget to add a KW_OnMouseOver handler.
 * \param   handler The KW_OnMouseOver function pointer.
 */
extern DECLSPEC void KW_AddWidgetMouseOverHandler(KW_Widget * widget, KW_OnMouseOver handler);

/**
 * \brief   Remove a KW_OnMouseOver handler from a widget
 * \details If you're not interested anymore in MouseOver events, remove your handler.
 * \param   widget The widget to remove the KW_OnMouseOver handler.
 * \param   handler The KW_OnMouseOver function pointer.
 */
extern DECLSPEC void KW_RemoveWidgetMouseOverHandler(KW_Widget * widget, KW_OnMouseOver handler);


/**
 * \brief   Adds a function that will be called whenever the mouse/cursor leaves the widget.
 * \details If you want to know when the cursor lefts the widget geometry, you can add a handler to the list.
 * \param   widget The widget to add a KW_OnMouseLeave handler.
 * \param   handler The KW_OnMouseLeave function pointer.
 */
extern DECLSPEC void KW_AddWidgetMouseLeaveHandler(KW_Widget * widget, KW_OnMouseLeave handler);

/**
 * \brief   Remove a KW_OnMouseLeave handler from a widget
 * \details If you're not interested anymore in MouseLeave events, remove your handler.
 * \param   widget The widget to remove the KW_OnMouseLeave handler.
 * \param   handler The KW_OnMouseLeave function pointer.
 */
extern DECLSPEC void KW_RemoveWidgetMouseLeaveHandler(KW_Widget * widget, KW_OnMouseLeave handler);

/**
 * \brief   Adds a function that will be called whenever the mouse/cursor clicks the widget.
 * \details If you want to know when the user clicked in your widget and with which button, insert a callback :)
 * \param   widget The widget to add a KW_OnMouseDown handler.
 * \param   handler The KW_OnMouseDown function pointer.
 */
extern DECLSPEC void KW_AddWidgetMouseDownHandler(KW_Widget * widget, KW_OnMouseDown handler);

/**
 * \brief   Remove a KW_OnMouseDown handler from a widget.
 * \details If you're not interested anymore in MouseDown events, remove your handler.
 * \param   widget The widget to remove the KW_OnMouseDown handler.
 * \param   handler The KW_OnMouseDown function pointer.
 */
extern DECLSPEC void KW_RemoveWidgetMouseDownHandler(KW_Widget * widget, KW_OnMouseDown handler);

/**
 * \brief   Adds a function that will be called whenever the mouse/cursor un-clicks the widget.
 * \details If you want to know when the user released the cursor in your widget and with which button, insert a callback :)
 * \param   widget The widget to add a KW_OnMouseUp handler.
 * \param   handler The KW_OnMouseUp function pointer.
 */
extern DECLSPEC void KW_AddWidgetMouseUpHandler(KW_Widget * widget, KW_OnMouseUp handler);

/**
 * \brief   Remove a KW_OnMouseUp handler from a widget.
 * \details If you're not interested anymore in KW_OnMouseUp events, remove your handler.
 * \param   widget The widget to remove the KW_OnMouseUp handler.
 * \param   handler The KW_OnMouseUp function pointer.
 */
extern DECLSPEC void KW_RemoveWidgetMouseUpHandler(KW_Widget * widget, KW_OnMouseUp handler);


/**
 * \brief   Adds a function that will be called whenever the focus is gained by this widget.
 * \details Focus usually happens when the user clicks on the widget or navigates via tab key.
 * \param   widget The widget to add a FocusGain handler.
 * \param   handler The OnFocusGain function pointer.
 */
extern DECLSPEC void KW_AddWidgetFocusGainHandler(KW_Widget * widget, KW_OnFocusGain handler);

/**
 * \brief   Remove a KW_OnFocusGain handler from a widget.
 * \details If you're not interested anymore in FocusGain events, remove your handler.
 * \param   widget The widget to remove the KW_OnFocusGain handler.
 * \param   handler The KW_OnFocusGain function pointer.
 */
extern DECLSPEC void KW_RemoveWidgetFocusGainHandler(KW_Widget * widget, KW_OnFocusGain handler);



/**
 * \brief   Adds a function that will be called whenever the focus moves from this to another widget.
 * \details If you want to be notified when the widget loses the focus, you should add a handler to it.
 * \param   widget The widget to add a FocusLose handler.
 * \param   handler The OnFocusLose function pointer.
 */
extern DECLSPEC void KW_AddWidgetFocusLoseHandler(KW_Widget * widget, KW_OnFocusLose handler);


/**
 * \brief   Remove a KW_OnFocusLose handler from a widget.
 * \details If you're not interested anymore in FocusLose events, remove your handler.
 * \param   widget The widget to remove the KW_OnFocusLose handler.
 * \param   handler The KW_OnFocusLose function pointer.
 */
extern DECLSPEC void KW_RemoveWidgetFocusLoseHandler(KW_Widget * widget, KW_OnFocusLose handler);

/**
 * \brief   Change the tileset used to render this widget.
 * \details You are still responsible for freeing the previous tileset.
 * \param   widget The widget to change the tileset.
 * \param   tileset The new tileset
 */
extern DECLSPEC void KW_SetWidgetTileset(KW_Widget * widget, SDL_Texture * tileset);

/**
 * \brief   Returns the current tileset being used by the widget.
 * \details Keep in mind that this can be the same of the set in the KW_GUI instance.
 * \param   widget The widget to retrieve the tileset from
 * \returns A pointer to the SDL_Texture being used as a tileset.
 */
extern DECLSPEC SDL_Texture * KW_GetWidgetTileset(KW_Widget * widget);

#ifdef __cplusplus
}
#endif

#endif