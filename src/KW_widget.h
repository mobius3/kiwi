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

#ifndef KW_WIDGET_H
#define KW_WIDGET_H

#include "KW_macros.h"
#include "KW_renderdriver.h"
#include "KW_bool.h"
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
extern DECLSPEC KW_RenderDriver * KW_GetRenderer(KW_GUI * gui);
extern DECLSPEC KW_GUI * KW_GetGUI(const KW_Widget * widget);



typedef void (*KW_WidgetPaintFunction)(KW_Widget * widget, const KW_Rect * absolute, void * data);
typedef void (*KW_WidgetDestroyFunction)(KW_Widget * widget);

typedef KW_Texture* (*KW_CustomRenderFunction)(KW_RenderDriver * renderer, KW_Widget * widget, KW_Surface * tileset, int w, int h);

/* mouse callbacks */
typedef void (*KW_OnMouseOver)(KW_Widget * widget);
typedef void (*KW_OnMouseLeave)(KW_Widget * widget);
typedef void (*KW_OnMouseDown)(KW_Widget * widget, int button);
typedef void (*KW_OnMouseUp)(KW_Widget * widget, int button);

/* drag callbacks */
typedef void (*KW_OnDragStart)(KW_Widget * widget, int x, int y);
typedef void (*KW_OnDragStop)(KW_Widget * widget, int x, int y);
typedef void (*KW_OnDrag)(KW_Widget * widget, int x, int y, int relx, int rely);

/* focus callbacks */
typedef void (*KW_OnFocusGain)(KW_Widget * widget);
typedef void (*KW_OnFocusLose)(KW_Widget * widget);

/* text and keyboard callbacks */
typedef void (*KW_OnTextInput)(KW_Widget * widget, const char * text);
typedef void (*KW_OnKeyDown)(KW_Widget * widget, SDL_Keycode sym, SDL_Scancode code);
typedef void (*KW_OnKeyUp)(KW_Widget * widget, SDL_Keycode sym, SDL_Scancode code);

/* widget internal events */
typedef void (*KW_OnGeometryChange)(KW_Widget * widget, const KW_Rect * newgeom, const KW_Rect * oldgeom);
typedef void (*KW_OnWidgetTilesetChange)(KW_Widget * widget);

/**
 * \brief   The event type when the children array changes.
 * \details Whenever the direct children number of an array changes, this
 *          enum is used to indicate if it was insertion or a removal.
 */
typedef enum KW_WidgetChildrenChangeEvent {
  KW_CHILDRENCHANGE_ADDED,
  KW_CHILDRENCHANGE_REMOVED
} KW_WidgetChildrenChangeEvent;

typedef void (*KW_OnWidgetChildrenChange)(KW_Widget * widget, KW_WidgetChildrenChangeEvent what, KW_Widget * child);

/**
 * \brief   Set of hints to control how this widget operates.
 * \details These hints can be queried by the widget implementation to change
 *          its behaviour. They are also used to change KiWi's behaviour when
 *          propagating input events, etc.
 */
typedef enum KW_WidgetHint {
  /** Allow widgets to stretch tiles */
  KW_WIDGETHINT_ALLOWTILESTRETCH = 1 << 0, 
  
  /** Makes KiWi avoid reporting input events to this widget and its children
   *  widget */
  KW_WIDGETHINT_BLOCKINPUTEVENTS = 1 << 1,
  
  /** Makes KiWi avoid reporting input event to this widget but still passes to
   * its children widgets */
  KW_WIDGETHINT_IGNOREINPUTEVENTS = 1 << 2,
  
  /** Hints widget implementations that, if possible, the user wants a
   * frameless version of it */
  KW_WIDGETHINT_FRAMELESS = 1 << 3,

  /** Makes KiWi not paint this widget */
  KW_WIDGETHINT_HIDDEN = 1 << 4,

  /** Makes KiWi draw debug information over this widget */
  KW_WIDGETHINT_DEBUG = 1 << 5
} KW_WidgetHint;

/**
 * \brief   Creates a new custom widget instance.
 * \details If you want to provide a new custom widget this is the function that you should call.
 *          All KW_Widget instances are built using this function, if you should need additional widget
 *          information you can use the data parameter.
 *          
 *          The widgetpaint function is called to paint the widget after the parent widget has been painted, never
 *          before, so you are garanteed to be rendering on top of it. It also uniquely identifies the kind of created
 *          widget (meaning that the paint function address is taken as the widget type).
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
 * \param   geometry The geometry of this widget *relative* to its parent.
 * \param   widgetpaing The paint function of this widget.
 * \param   widgetdestroy The destroy function of this widget.
 * \param   data Any private data associated with this widget. Mostly to be used by widget implementations.
 * \returns The new widget.
 */
extern DECLSPEC KW_Widget * KW_CreateWidget(KW_GUI * gui, 
                                              KW_Widget * parent, 
                                              const KW_Rect * geometry,
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
extern DECLSPEC KW_Widget * KW_GetWidgetParent(const KW_Widget * widget);

/**
 * \brief      Returns an array of all childrens of this widget.
 * \details    The returned array is the same as stored internally and its contents
 *             can change if any reparenting is done after calling this function,
 *             thus, you should not store its value.
* \param       widget The widget to return childrens from.
 * \param[out] count If not NULL, the unsigned int pointed by it will have the
 *             children count of the widget.
 * \returns    An array of constant pointers to all childrens of \p widget.
 */
extern DECLSPEC KW_Widget * const * KW_GetWidgetChildren(const KW_Widget * widget, unsigned int * count);

/**
 * \brief   Retrieves the current data associated with a widget.
 * \details If you are implementing a custom widget, you must call this function to access the data you
 *          set up previously when the widget was created.
 * \param   widget The widget to retrieve the data from.
 * \param   type The address of the widget paint function (to confirm that you actually know this widget)
 * \returns The data pointer or NULL if the widget types don't match.
 */
extern DECLSPEC void * KW_GetWidgetData(const KW_Widget * widget, KW_WidgetPaintFunction paint);


/**
 * \brief   Sets a pointer to user-specific data to be read later.
 * \details Use this if you want to store user-specific state in this widget
 *          that could be shared among callbacks. Widget implementations should
 *          not use this. There's no check if it had any user data before, use
 *          with caution.
 * \param   widget The widget to set the user data.
 * \param   userdata The associated user data
 */
extern DECLSPEC void KW_SetWidgetUserData(KW_Widget * widget, void * userdata);

/**
 * \brief   Returns the associated user data of a widget.
 * \details Use this function to access a previously stored user data pointer.
 * \param   widget The widget to get userdata from.
 * \returns The usertdata pointer
 */
extern DECLSPEC void * KW_GetWidgetUserData(const KW_Widget * widget);

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
extern DECLSPEC KW_GUI * KW_GetWidgetGUI(const KW_Widget * widget);

/**
 * \brief   Returns the KW_RenderDriver associated with a KW_GUI instance that the @p widget is associated with.
 * \details This is a helper function and its effects are the same of `KW_GetRenderer(KW_GetWidgetGUI(widget))`
 * \param   widget The widget to get the KW_RenderDriver instance from.
 * \returns The associated KW_RenderDriver instance.
 */
extern DECLSPEC KW_RenderDriver * KW_GetWidgetRenderer(const KW_Widget * widget);

/**
 * \brief		Get custom draw-widget function
 * \details Call this function from within a KW_OnGeometryChange handler to update image of the widget.
 *          if no such function specified forf the widget, default widget renderer will be called instead.
 * \param   widget The widget to get function from
 * \returns Address of the rendering function for the widget or NULL if no such function specified.
 */
extern DECLSPEC KW_CustomRenderFunction KW_GetWidgetCustomRenderFunction(const KW_Widget * widget);

/**
 * \brief   Set custom function for widget rendering
 * \details If this function is not specified default rendering function for the widget will be called
 *          to update widget's image after resizing 
 * \param   widget The widget whose renderer function you need to change
 * \param   renderfunction The function to be called to obtain widget's face on resizing
 */
extern DECLSPEC void KW_SetWidgetCustomRenderFunction(KW_Widget * widget, KW_CustomRenderFunction renderfunction);

/**
 * \brief   Sets a new geometry for a widget.
 * \details Use this function if you need to change the widget relative position or its size.
 * \param   widget The widget that will have its size changed.
 * \param   geometry The new widget geometry.
 */
extern DECLSPEC void KW_SetWidgetGeometry(KW_Widget * widget, const KW_Rect * geometry);

/**
 * \brief   Gets the widget geometry.
 * \details Use this function to retrieve the current *relative* geometry of the widget.
 * \param   widget The widget to get the geometry from.
 * \param   geometry A KW_Rect pointer that will receive the @p widget geometry.
 */
extern DECLSPEC void KW_GetWidgetGeometry(const KW_Widget * widget, KW_Rect * geometry);

/**
 * \brief   Returns the widget geometry.
 * \details Use this function to retrieve the current *relative* geometry of the widget.
 * \param   widget The widget to get the geometry from.
 * \returns A KW_Rect pointer that will point to the the @p widget geometry.
 */
extern DECLSPEC const KW_Rect * KW_ReturnWidgetGeometry(const KW_Widget * widget);

/**
 * \brief   Gets the absolute widget geometry.
 * \details Use this if you need to know the absolute widget geometry calculated taking its parents in consideration.
 *          It is useful inside the paint function of a widget.
 * \param   widget The widget to get the absolute geometry from.
 * \param   geometry A KW_Rect pointer that will receive the @p widget absolute geometry.
 */
extern DECLSPEC void KW_GetWidgetAbsoluteGeometry(const KW_Widget * widget, KW_Rect * geometry);

/**
 * \brief   Gets the composed geometry of a widget.
 * \details   Returns the total area occupied by a widget and its children. Note that the position is
 *          still relative to its parent widget.
 * \param   widget The widget to get the compsoed geometry from.
 * \param   geometry A KW_Rect pointer that will receive the @p widget composed geometry.
 */
extern DECLSPEC void KW_GetWidgetComposedGeometry(const KW_Widget * widget, KW_Rect * composed);

/**
 * \brief   Paint a widget and all of its children unless they are hidden.
 * \details Iterates through all of the @p widget subtree calling their paint functions.
 *          If you wish to paint all widgets of a KW_GUI instance it is better to call
 *          `KW_Paint()` rather than this function.
 * \param   widget The widget to paint.
 */
extern DECLSPEC void KW_PaintWidget(KW_Widget * widget);

/**
 * \brief   Moves widget at the back of the rendering list.
 * \details Iterates through all of the @p widget subtree searching for a widget to
            put it at the back of chidlren to render makeing it effectivly be on top.
 * \param   widget The widget to apply changes to.
 */
extern DECLSPEC void KW_BringToFront(KW_Widget * widget);

/**
 * \brief   Moves GUI focus to given widget.
 * \details Make given widget target of user input, notify previous target and fire events.
 * \param   widget The widget to set focused.
 */
extern DECLSPEC void KW_SetFocusedWidget(KW_Widget * widget);

/**
 * \brief   Hides this widget and their children, effectively preventing them from being painted.
 * \details You can use this function if you want to prevent the paint function of the widget from
 *          being called. Please note that this also disables event dispatching for this widget and
 *          their children.
 *
 *          *This is just a wrapper for KW_EnableWidgetHint(widget, KW_WIDGETHINT_HIDDEN, KW_TRUE)*
 *
 * \param   widget The widget to hide.
 */
extern DECLSPEC void KW_HideWidget(KW_Widget * widget);

/**
 * \brief   Shows a widget that was hidden.
 * \details See ::KW_HideWidget. This enables the painting of this widget (and childrens) in the next
 *          ::KW_Paint call.
 *
 *          *This is just a wrapper of KW_DisableWidgetHint(w, KW_WIDGETHINT_HIDDEN)*
 * \param   widget The widget that will now be painted.
 */
extern DECLSPEC void KW_ShowWidget(KW_Widget * widget);

/**
 * \brief   Returns KW_TRUE if this widget is hidden.
 * \param   widget The widget to query.
 * \details This function is a wrapper of KW_QueryWidgetHint(w, KW_WIDGETHINT_HIDDEN)
 * \return  KW_TRUE or KW_FALSE if hidden or not.
 */
extern DECLSPEC KW_bool KW_IsWidgetHidden(KW_Widget * widget);

/**
 * \brief   Enable debug painting for this widget (and optionally its children);
 * \details Use this funcion if you want to debug rhe resulting geometry of your widget.
 *
 *          *This is just a wrapper for KW_EnableWidgetHint(widget, KW_WIDGETHINT_DEBUG)*
 *
 * \param   widget The widget to hide.
 * \param   enableInChildren If debug should be enabled in children widgets
 */
extern DECLSPEC void KW_EnableWidgetDebug(KW_Widget * widget, KW_bool enableInChildren);

/**
 * \brief   Disables painting debug information for this widget
 * \details See ::KW_EnableWidgetDebug.
 *
 *          *This is just a wrapper of KW_DisableWidgetHint(w, KW_WIDGETHINT_DEBUG)*
 *
 * \param   widget The widget will no longer paint debug info.
 * \param   disableInChildren If debug should be disabled in children widgets
 */
extern DECLSPEC void KW_DisableWidgetDebug(KW_Widget * widget, KW_bool disableInChildren);

/**
 * \brief   Returns KW_TRUE if this widget should paint debug info.
 * \param   widget The widget to query.
 * \details This function is a wrapper of KW_QueryWidgetHint(w, KW_WIDGETHINT_DEBUG)
 * \return  KW_TRUE or KW_FALSE if debugging or not.
 */
extern DECLSPEC KW_bool KW_IsDebugWidgetEnabled(KW_Widget * widget);

/* Stuff now related to handling of events (focus, mouse, keyboard, etc) */

/**
 * \brief   Block this widget from receiving input events (mouse, keyboard, touch, etc)
 * \details You can use this function if you want to keep the widget from receiving input related events.
 *          The widget will not know if the cursor is over it, if a key was pressed, etc.
 *         
 *          If you have composite widgets and want the widgets on top to not steal mouse-over or press events
 *          from the widgets below this function might be useful to you.
 * 
 *          All children widgets will also have its input events blocked.
 *          *This is just a wrapper of KW_EnableWidgetHint(w, KW_WIDGETHINT_BLOCKINPUTEVENTS, KW_FALSE)*
 * \param   widget The widget that will stop receiving input evenets.
 */
extern DECLSPEC void KW_BlockWidgetInputEvents(KW_Widget * widget);

/**
 * \brief   Unblocks this widget from receiving input events.
 * \details See ::KW_BlockWidgetEvents. All children widgets will have its input events unblocked.
 *          *This is just a wrapper of KW_DisableWidgetHint(w, KW_WIDGETHINT_BLOCKINPUTEVENTS, KW_FALSE)*
 * \param   widget The widget that will now receive input events.
 */
extern DECLSPEC void KW_UnblockWidgetInputEvents(KW_Widget * widget);

/**
 * \brief   Returns KW_TRUE if this widget is blocking input events.
 * \param   widget The widget to query.
 * \details This function is a wrapper of KW_QueryWidgetHint(w, KW_WIDGETHINT_BLOCKINPUTEVENTS)
 * \return  KW_TRUE or KW_FALSE in case events are blocked or not.
 */
extern DECLSPEC KW_bool KW_IsWidgetInputEventsBlocked(KW_Widget * widget);

extern DECLSPEC void KW_EnableWidgetHint(KW_Widget * widget, KW_WidgetHint hint, KW_bool down);
extern DECLSPEC void KW_DisableWidgetHint(KW_Widget * widget, KW_WidgetHint hint, KW_bool down);
extern DECLSPEC KW_bool KW_QueryWidgetHint(const KW_Widget * widget, KW_WidgetHint hint);

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
 * \brief   Adds a function that will be called when there's new text input.
 * \details This handler will *only* be called if this widget is the focused widget *and* you called SDL_StartTextInput().
 * \param   widget The widget to add a KW_OnTextInput handler.
 * \param   handler The KW_OnTextInput function pointer.
 */
extern DECLSPEC void KW_AddWidgetTextInputHandler(KW_Widget * widget, KW_OnTextInput handler);


/**
 * \brief   Remove a KW_OnTextInput handler from a widget.
 * \details If you're not interested anymore in KW_OnTextInput events, remove your handler.
 * \param   widget The widget to remove the KW_OnTextInput handler.
 * \param   handler The KW_OnTextInput function pointer.
 */
extern DECLSPEC void KW_RemoveWidgetTextInputHandler(KW_Widget * widget, KW_OnTextInput handler);


/**
 * \brief   Adds a function that will be called when there's a key pressed.
 * \details This handler will *only* be called if this widget is the focused widget.
 * \param   widget The widget to add a KW_OnKeyDown handler.
 * \param   handler The KW_OnKeyDown function pointer.
 */
extern DECLSPEC void KW_AddWidgetKeyDownHandler(KW_Widget * widget, KW_OnKeyDown handler);


/**
 * \brief   Remove a KW_OnKeyDown handler from a widget.
 * \details If you're not interested anymore in KW_OnKeyDown events, remove your handler.
 * \param   widget The widget to remove the KW_OnKeyDown handler.
 * \param   handler The KW_OnKeyDown function pointer.
 */
extern DECLSPEC void KW_RemoveWidgetKeyDownHandler(KW_Widget * widget, KW_OnKeyDown handler);

/**
 * \brief   Adds a function that will be called when there's a key released.
 * \details This handler will *only* be called if this widget is the focused widget.
 * \param   widget The widget to add a KW_OnKeyUp handler.
 * \param   handler The KW_OnKeyUp function pointer.
 */
extern DECLSPEC void KW_AddWidgetKeyUpHandler(KW_Widget * widget, KW_OnKeyUp handler);


/**
 * \brief   Remove a KW_OnKeyUp handler from a widget.
 * \details If you're not interested anymore in KW_OnKeyUp events, remove your handler.
 * \param   widget The widget to remove the KW_OnKeyUp handler.
 * \param   handler The KW_OnKeyUp function pointer.
 */
extern DECLSPEC void KW_RemoveWidgetKeyUpHandler(KW_Widget * widget, KW_OnKeyUp handler);

/**
 * \brief   Adds a function that will be called when drag is starting over the widget.
 * \details This handler is called after the cursor is pressed and then moved.
 * \param   widget The widget to add a KW_OnDragStart handler.
 * \param   handler The KW_OnDragStart function pointer.
 */
extern DECLSPEC void KW_AddWidgetDragStartHandler(KW_Widget * widget, KW_OnDragStart handler);


/**
 * \brief   Remove a KW_OnDragStart handler from a widget.
 * \details If you're not interested anymore in KW_OnDragStart events, remove your handler.
 * \param   widget The widget to remove the KW_OnDragStart handler.
 * \param   handler The KW_OnDragStart function pointer.
 */
extern DECLSPEC void KW_RemoveWidgetDragStartHandler(KW_Widget * widget, KW_OnDragStart handler);

/**
 * \brief   Adds a function to be called when a drag stops.
 * \details The handler will be called only if the drag was started over this widget.
 * \param   widget The widget to add a KW_OnDragStop handler.
 * \param   handler The KW_OnDragStop function pointer.
 */
extern DECLSPEC void KW_AddWidgetDragStopHandler(KW_Widget * widget, KW_OnDragStop handler);


/**
 * \brief   Remove a KW_OnDragStop handler from a widget.
 * \details If you're not interested anymore in KW_OnDragStop events, remove your handler.
 * \param   widget The widget to remove the KW_OnDragStop handler.
 * \param   handler The KW_OnDragStop function pointer.
 */
extern DECLSPEC void KW_RemoveWidgetDragStopHandler(KW_Widget * widget, KW_OnDragStop handler);

/**
 * \brief   Adds a function to be called when a drag event occurs.
 * \details The handler will be called only if the drag was started over this widget and the DragStart handler was called.
 * \param   widget The widget to add a KW_OnDragStop handler.
 * \param   handler The KW_OnDragStop function pointer.
 */
extern DECLSPEC void KW_AddWidgetDragHandler(KW_Widget * widget, KW_OnDrag handler);

/**
 * \brief   Adds a function to be called when this widget geometry is changed.
 * \param   widget The widget that has its geometry changed.
 * \param   handler The KW_OnGeometryChange function handler.
 */
extern DECLSPEC void KW_AddWidgetGeometryChangeHandler(KW_Widget * widget, KW_OnGeometryChange handler);

/**
 * \brief   Remove a KW_OnGeometryChange handler.
 * \param   widget The widget to remove the KW_OnGeometryChange handler.
 * \param   handler The KW_OnGeometryChange function pointer.
 */
extern DECLSPEC void KW_RemoveWidgetGeometryChangeHandler(KW_Widget * widget, KW_OnGeometryChange handler);

/**
 * \brief   Adds a function to call when this widget children count changes.
 * \param   widget The widget that has its children count changed.
 * \param   handler The KW_OnWidgetChildrenChange function handler.
 */
extern DECLSPEC void KW_AddWidgetChildrenChangeHandler(KW_Widget * widget, KW_OnWidgetChildrenChange handler);

/**
 * \brief   Remove a KW_OnWidgetChildrenChange handler.
 * \param   widget The widget to remove the KW_OnWidgetChildrenChange handler.
 * \param   handler The KW_OnWidgetChildrenChange function pointer.
 */
extern DECLSPEC void KW_RemoveWidgetChildrenChangeHandler(KW_Widget * widget, KW_OnWidgetChildrenChange handler);

/**
 * \brief   Remove a KW_OnDrag handler from a widget.
 * \details If you're not interested anymore in KW_OnDrag events, remove your handler.
 * \param   widget The widget to remove the KW_OnDrag handler.
 * \param   handler The KW_OnDrag function pointer.
 */
extern DECLSPEC void KW_RemoveWidgetDragHandler(KW_Widget * widget, KW_OnDrag handler);

extern DECLSPEC void KW_AddWidgetTilesetChangeHandler(KW_Widget * widget, KW_OnWidgetTilesetChange handler);
extern DECLSPEC void KW_RemoveWidgetTilesetChangeHandler(KW_Widget * widget, KW_OnWidgetTilesetChange handler);

/**
 * \brief   Change the tileset used to render this widget.
 * \details You are still responsible for freeing the previous tileset.
 * \param   widget The widget to change the tileset.
 * \param   tileset The new tileset surface.
 */
extern DECLSPEC void KW_SetWidgetTilesetSurface(KW_Widget * widget, KW_Surface * tileset);

/**
 * \brief   Returns the current tileset being used by the widget.
 * \details Keep in mind that this can be the same of the set in the KW_GUI instance.
 * \param   widget The widget to retrieve the tileset from.
 * \returns A pointer to the KW_Texture being used as a tileset.
 */
extern DECLSPEC KW_Texture * KW_GetWidgetTilesetTexture(KW_Widget * widget);

/**
 * \brief   Returns the current tileset being used by the widget.
 * \details Keep in mind that this can be the same of the set in the KW_GUI instance.
 * \param   widget The widget to retrieve the tileset surface from.
 * \returns A pointer to the KW_Texture being used as a tileset.
 */
extern DECLSPEC KW_Surface * KW_GetWidgetTilesetSurface(KW_Widget * widget);

/**
 * \brief   Sets this widget to clip its children to its geometry.
 * \details Both rendering and event calculation/reporting will be blocked for
 *          ALL the widgets in the tree.
 * \param   widget The widget that will have its children clipped or not.
 * \param   shouldclip If it should clip or not.
 */
extern DECLSPEC void KW_SetClipChildrenWidgets(KW_Widget * widget, KW_bool shouldclip);

/**
 * \brief Checks if there's a cursor over the widget.
 * \details This does not takes in consideration parent widgets (e.g, if cursor is over a widget,
 *          calling this function on its parent would return KW_FALSE)
 * \param widget The widget to check if the cursor is over
 * \returns KW_TRUE if the cursor is over, KW_FALSE otherwise
 */
extern DECLSPEC KW_bool KW_IsCursorOverWidget(KW_Widget * widget);

/**
 * \brief Checks if there's a cursor pressing on the widget
 * \param widget The widget to check if the cursor is pressing on
 * \returns KW_TRUE if the cursor is pressing, KW_FALSE otherwise
 */
extern DECLSPEC KW_bool KW_IsCursorPressedOnWidget(KW_Widget * widget);

/**
 * \brief Checks if the cursor was pressing and was just released.
 * \details This is only true on the same paint/update cycle that the cursor was
 *          released.
 * \param widget The widget to check if the cursor is released
 * \returns KW_TRUE if the cursor is released, KW_FALSE otherwise
 */
extern DECLSPEC KW_bool KW_IsCursorReleasedOnWidget(KW_Widget * widget);

#ifdef __cplusplus
}
#endif

#endif
