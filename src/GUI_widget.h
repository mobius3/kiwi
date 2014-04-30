#ifndef GUI_WIDGET_H
#define GUI_WIDGET_H

#include "GUI_macros.h"
#include "SDL.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief   The GUI_Widget type represents a widget on the GUI_GUI instance.
 * \details Regardless of the widget type, they are all represented by this opaque type.
 */
struct GUI_Widget;
typedef struct GUI_Widget GUI_Widget;


struct GUI_GUI;
typedef struct GUI_GUI GUI_GUI;

/* forward declarations of these */
extern DECLSPEC SDL_Renderer * GUI_GetRenderer(GUI_GUI * gui);
extern DECLSPEC GUI_GUI * GUI_GetGUI(GUI_Widget * widget);



typedef void (*GUI_WidgetPaintFunction)(GUI_Widget * widget);
typedef void (*GUI_WidgetDestroyFunction)(GUI_Widget * widget);

/**
 * \brief   The GUI_WidgetType enumeration represents available widget types.
 * \details Every widget created must set a widget type even if its a custom widget.
 */
typedef enum GUI_WidgetType {
  GUI_WIDGETTYPE_NONE,
  GUI_WIDGETTYPE_FRAME,
  GUI_WIDGETTYPE_LABEL,
  GUI_WIDGETTYPE_BUTTON,
  GUI_WIDGETTYPE_CUSTOM0 = 0x100000,
  GUI_WIDGETTYPE_CUSTOM1,
  GUI_WIDGETTYPE_CUSTOM2,
  GUI_WIDGETTYPE_CUSTOM3,
  GUI_WIDGETTYPE_CUSTOM4,
  GUI_WIDGETTYPE_CUSTOM5,
  GUI_WIDGETTYPE_CUSTOM6,
  GUI_WIDGETTYPE_CUSTOM7,
  GUI_WIDGETTYPE_CUSTOM8,
  GUI_WIDGETTYPE_CUSTOM9,
  GUI_WIDGETTYPE_CUSTOM10,
  GUI_WIDGETTYPE_CUSTOM11,
  GUI_WIDGETTYPE_CUSTOM12,
  GUI_WIDGETTYPE_CUSTOM13,
  GUI_WIDGETTYPE_CUSTOM14,
  GUI_WIDGETTYPE_CUSTOM15,
  GUI_WIDGETTYPE_CUSTOM16,
  GUI_WIDGETTYPE_CUSTOM17,
  GUI_WIDGETTYPE_CUSTOM18,
  GUI_WIDGETTYPE_CUSTOM19,
  GUI_WIDGETTYPE_CUSTOM20
} GUI_WidgetType;


/**
 * \brief   Creates a new custom widget instance.
 * \details If you want to provide a new custom widget this is the function that you should call.
 *          All GUI_Widget instances are built using this class, if you should need additional widget 
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
 * \param   gui The GUI_GUI instace that will hold this widget in its widget tree.
 * \param   parent The parent widget.
 * \param   type The type of this widget.
 * \param   geometry The geometry of this widget *relative* to its parent.
 * \param   widgetpaing The paint function of this widget.
 * \param   widgetdestroy The destroy function of this widget.
 * \param   data Any private data associated with this widget.
 * \returns The new widget.
 */
extern DECLSPEC GUI_Widget * GUI_CreateWidget(GUI_GUI * gui, 
                                              GUI_Widget * parent, 
                                              GUI_WidgetType type, 
                                              const SDL_Rect * geometry, 
                                              GUI_WidgetPaintFunction widgetpaint,
                                              GUI_WidgetDestroyFunction widgetdestroy,
                                              void * data);


/**
 * \brief   Change the parent of a widget to another parent.
 * \details Remove \p widget as a child from its current parent and adds it as a child to \p parent.
 *          Every child of \p widget will then belong to the new \p parent tree. Note that \p widget
 *          geometry is **NOT** updated to keep it in the previous absolute position. You should call
 *          GUI_SetWidgetGeometry() to update it.
 * 
 *          Keep in mind that this is one of the costliest functions to call in the GuiGUI library as it
 *          might involve memory reallocation both in the previous and in the new parent widget. The composed
 *          geometry also needs to be updated in both parents in a chain that might go as high as the root widget.
 *          You should avoid widget reparenting if you can.
 * 
 *          To make the widget a top-level widget you should pass \c NULL as the @p parent parameter.
 * 
 * \param   widget The widget that will have its parent changed.
 * \param   parent The new \p widget parent.
 */
extern DECLSPEC void GUI_ReparentWidget(GUI_Widget * widget, GUI_Widget * parent);

/**
 * \brief   Retrieves the parent widget of a widget.
 * \param   widget The GUI_Widget instance to retrieve the parent from.
 * \returns The parent widget or NULL if its a top-level widget.
 */
extern DECLSPEC GUI_Widget * GUI_GetWidgetParent(GUI_Widget * widget);

/**
 * \brief   Retrieves the current data associated with a widget.
 * \details If you are implementing a custom widget, you must call this function to access the data you
 *          set up previously when the widget was created.
 * \param   widget The widget to retrieve the data from.
 * \returns The data pointer.
 */
extern DECLSPEC void * GUI_GetWidgetData(GUI_Widget * widget);

/**
 * \brief   Destroys a widget and free its resources.
 * \details Any custom resource that you set (fonts, custom textures, etc) needs to be freed by you unless specified otherwise.
 * \param   widget The widget to destroy.
 * \param   destroychildren Set to 1 if you want also to destroy all childrens (and their childrens) of this widget. If not, they will be
 *                          reparented to @p widget parent.
 */
extern DECLSPEC void GUI_DestroyWidget(GUI_Widget * widget, int destroychildren);

/**
 * \brief   Returns the associated GUI_GUI instance of a widget.
 * \details If you have a widget instance, this is the means of retrieving its GUI_GUI instance.
 * \param   widget The widget to get the GUI_GUI instance from.
 * \return  The GUI_GUI instance associated with the widget.
 */
extern DECLSPEC GUI_GUI * GUI_GetWidgetGUI(GUI_Widget * widget);

/**
 * \brief   Returns the SDL_Renderer associated with a GUI_GUI instance that the @p widget is associated with.
 * \details This is a helper function and its effects are the same of `GUI_GetRenderer(GUI_GetWidgetGUI(widget))`
 * \param   widget The widget to get the SDL_Renderer instance from.
 * \returns The associated SDL_Renderer instance.
 */
extern DECLSPEC SDL_Renderer * GUI_GetWidgetRenderer(GUI_Widget * widget);

/**
 * \brief   Sets a new geometry for a widget.
 * \details Use this function if you need to change the widget relative position or its size.
 * \param   widget The widget that will have its size changed.
 * \param   geometry The new widget geometry.
 */
extern DECLSPEC void GUI_SetWidgetGeometry(GUI_Widget * widget, const SDL_Rect * geometry);

/**
 * \brief   Gets the widget geometry.
 * \details Use this function to retrieve the current *relative* geometry of the widget.
 * \param   widget The widget to get the geometry from.
 * \param   geometry A SDL_Rect pointer that will receive the @p widget geometry. 
 */
extern DECLSPEC void GUI_GetWidgetGeometry(GUI_Widget * widget, SDL_Rect * geometry);

/**
 * \brief   Gets the absolute widget geometry.
 * \details Use this if you need to know the absolute widget geometry calculated taking its parents in consideration.
 *          It is useful inside the paint function of a widget.
 * \param   widget The widget to get the absolute geometry from.
 * \param   geometry A SDL_Rect pointer that will receive the @p widget absolute geometry.
 */
extern DECLSPEC void GUI_GetWidgetAbsoluteGeometry(GUI_Widget * widget, SDL_Rect * geometry);

/**
 * \brief   Gets the composed geometry of a widget.
 * \details   Returns the total area occupied by a widget and its children. Note that the position is
 *          still relative to its parent widget.
 * \param   widget The widget to get the compsoed geometry from.
 * \param   geometry A SDL_Rect pointer that will receive the @p widget composed geometry.
 */
extern DECLSPEC void GUI_GetWidgetComposedGeometry(GUI_Widget * widget, SDL_Rect * composed);

/**
 * \brief   Paint a widget and all of its children unless they are hidden.
 * \details Iterates through all of the @p widget subtree calling their paint functions.
 *          If you wish to paint all widgets of a GUI_GUI instance it is better to call
 *          `GUI_Paint()` rather than this function.
 * \param   widget The widget to paint.
 */
extern DECLSPEC void GUI_PaintWidget(GUI_Widget * widget);

#ifdef __cplusplus
}
#endif

#endif