#ifndef QM_REBORN_H
#define QM_REBORN_H

#define QM_REBORN_INTERNAL
#ifdef __SNC__
#include <scetypes.h>
#else
#include <psp2/types.h>
#endif
#include "c_types.h"
#define CHECKBOX_HANDLER(name) void name(int checked)
#define BUTTON_HANDLER(name) void name()
#define ONLOAD_HANDLER(name) void name()

#ifdef __cplusplus
#define EXPORT extern "C"
#else
#define EXPORT extern
#endif

/**
 *
 * @brief                   Add a widget to the quick menu
 * 
 * @param       data        Pointer to a widgetData populated with values
 * 
 * @todo
 * @retval      0 on success < 0 on error
 * 
*/
EXPORT int QuickMenuRebornAddWidget(widgetData *data);

/**
 *
 * @brief                   Update an already added widget in the quick menu
 * 
 * @param       data           Pointer to widgetData populated with the values
 * @param       flags          Flags to specify exactly what to update
 * @todo
 * @retval      0 on success < 0 on error
 * 
*/
EXPORT int QuickMenuRebornUpdateWidget(widgetData *data, int flags);

/**
 *
 * @brief                   Remove an already added widget from the quick menu
 * 
 * @param       refID       refrence ID of widget
 * 
 * @todo
 * @retval      0 on success < 0 on error
 * 
*/
EXPORT int QuickMenuRebornRemoveWidget(const char *data);

/**
 *
 * @brief                   Remove a separator added with QucikMenuRebornSeparator
 * 
 * @param       refID       Refrence ID of separator
 * 
 * @todo
 * @retval      0 on succes < 0 on error
 * 
*/
EXPORT int QuickMenuRebornRemoveSeparator(const char *refID);

/**
 *
 * @brief                   Make a widgetColor and populate values
 * 
 * @param       r           Value to set at r
 * @param       g           Value to set at g
 * @param       b           Value to set at b
 * @param       a           Value to set at a
 * 
 * @todo
 * @retval      widgetColor
 * 
*/
EXPORT widgetColor makeWidgetColor(float r, float g, float b, float a);

/**
 *
 * @brief                   Make a vector4 and populate values
 * 
 * @param       x           Value to set at x
 * @param       y           Value to set at y
 * @param       z           Value to set at z
 * @param       w           Value to set at w
 * 
 * @todo
 * @retval      Vector4
 * 
*/
EXPORT vector4 makeWidgetVector4(float x, float y, float z, float w);

/**
 *
 * @brief                   Add a widget with advanced settings to the quick menu
 * 
 * @param       refID       Refrence ID of the widget
 * @param       Size        Pointer to size of widget
 * @param       Position    Pointer to position of widget
 * @param       Color       Pointer to color of widget
 * @param       Type        Pointer to buffer containg type info of the widget
 * @param       style       Pointet to buffer containg style info of the widget
 * @param       OnLoad      Function to be called whenever widget is loaded in quick menu
 * @todo
 * @retval      0 on succes < 0 on error
 * 
*/
EXPORT int QuickMenuRebornAddAdvancedWidget(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Type, const char *style, void (*OnLoad)());

#define COLOR_WHITE makeWidgetColor(1.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_RED makeWidgetColor(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_GREEN makeWidgetColor(0.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_BLUE makeWidgetColor(0.0f, 0.0f, 1.0f, 1.0f)
#define COLOR_YELLOW makeWidgetColor(1.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_LIGHT_BLUE makeWidgetColor(0.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_PINK makeWidgetColor(1.0f, 0, 1.0f, 1.0f)
#define COLOR_YELLOW makeWidgetColor(1.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_GRAY makeWidgetColor(0.5f, 0.5f, 0.5f, 0.5f)
#define COLOR_TRANSPARENT makeWidgetColor(1.0f, 1.0f, 1.0f, 0.0f)

#define makeCommonWidgetColor(common) makeWidgetColor(common, common, common, common)
#define makeCommonWidgetVector4(common) makeWidgetVector4(common, common, common, common)

#define makeWidgetVector4Int(x,y,z,w) makeWidgetVector4(x##.0f, y##.0f, z##.0f, w##.0f)
#define makeWidgetColorInt(r,g,b,a) makeWidgetColor(r##.0f, g##.0f, b##.0f, a##.0f)

#define makeCommonWidgetVector4Int(common) makeWidgetVector4(common##.0f, common##.0f, common##.0f, common##.0f)
#define makeCommonWidgetColorInt(common) makeWidgetColor(common##.0f, common##.0f, common##.0f, common##.0f)

//Size of all the other planes in the quick menu (as close as I could get)
#define SCE_PLANE_WIDTH 825.0f

/**
 *
 * @brief                   Add a button to the quick menu
 * 
 * @param       refID       Refrence ID of the widget
 * @param       parentRefID Refrence ID of the parent widget (pass NULL if none)
 * @param       Size        Pointer to size of widget
 * @param       Position    Pointer to position of widget
 * @param       Color       Pointer to color of widget
 * @param       OnLoad      Function to be called whenever widget is loaded in quick menu
 * @param       OnPress    Function to be called whenever the checkbox is pressed
 * @todo
 * @retval      0 on succes < 0 on error
 * 
*/
EXPORT int QuickMenuRebornButton(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void (*OnLoad)(), void(*OnPress)(void));

/**
 *
 * @brief                   Add a checkbox to the quick menu
 * 
 * @param       refID       Refrence ID of the widget
 * @param       parentRefID Refrence ID of the parent widget (pass NULL if none)
 * @param       Size        Pointer to size of widget
 * @param       Position    Pointer to position of widget
 * @param       Color       Pointer to color of widget
 * @param       OnLoad      Function to be called whenever widget is loaded in quick menu
 * @param       OnToggle    Function to be called whenever the checkbox is pressed (access the current state with the variable state)
 * @param       state       Initial state of Check Box when spawned, ON = On, OFF = Off, PREV_STATE = previous saved state (saved on reboot / poweroff)
 * @todo
 * @retval      0 on succes < 0 on error
 * 
*/
EXPORT int QuickMenuRebornCheckBox(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, void (*OnLoad)(), void(*OnToggle)(int state), CheckBoxState state);

/**
 *
 * @brief                   Add some text to the quick menu
 * 
 * @param       refID       Refrence ID of the widget
 * @param       parentRefID Refrence ID of the parent widget (pass NULL if none)
 * @param       Size        Pointer to size of widget
 * @param       Position    Pointer to position of widget
 * @param       Color       Pointer to color of widget
 * @param       Text        Text to be displayed
 * @param       OnLoad      Function to be called whenever widget is loaded in quick menu
 * @todo
 * @retval      0 on succes < 0 on error
 * 
*/
EXPORT int QuickMenuRebornText(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void (*OnLoad)());

/**
 *
 * @brief                   Add a plane (like a box) to the quick menu
 * 
 * @param       refID       Refrence ID of the widget
 * @param       parentRefID Refrence ID of the parent widget (pass NULL if none)
 * @param       Size        Pointer to size of widget
 * @param       Position    Pointer to position of widget
 * @param       Color       Pointer to color of widget
 * @param       OnLoad      Function to be called whenever widget is loaded in quick menu
 * @todo
 * @retval      0 on succes < 0 on error
 * 
*/
EXPORT int QuickMenuRebornPlane(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, void (*OnLoad)());

/**
 *
 * @brief                   Add a separator (line and space) to the quick menu
 * 
 * @param       refID       Refrence ID of the widget
 * @todo
 * @retval      0 on succes < 0 on error
 * 
*/
EXPORT int QuickMenuRebornSeparator(const char *refID);

/**
 *
 * @brief                   Update an already added button in the quick menu
 * 
 * @param       refID       Refrence ID of the widget
 * @param       Size        Pointer to size of widget
 * @param       Position    Pointer to position of widget
 * @param       Color       Pointer to color of widget
 * @param       OnLoad      Function to be called whenever widget is loaded in quick menu
 * @param       OnPress     Function to be called whenever the button is pressed
 * @param       flags       Flags that specify exactly what to update
 * @todo
 * @retval      0 on succes < 0 on error
 * 
*/
EXPORT int QuickMenuRebornUpdateButton(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void(*OnPress)(void), void (*OnLoad)(), int flags);

/**
 *
 * @brief                   Update an already added checkbox in the quick menu
 * 
 * @param       refID       Refrence ID of the widget
 * @param       Size        Pointer to size of widget
 * @param       Position    Pointer to position of widget
 * @param       Color       Pointer to color of widget
 * @param       OnLoad      Function to be called whenever widget is loaded in quick menu
 * @param       OnToggle    Function to be called whenever the checkbox is pressed
 * @param       flags       Flags that specify exactly what to update
 * @todo
 * @retval      0 on succes < 0 on error
 * 
*/
EXPORT int QuickMenuRebornUpdateCheckBox(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, void(*OnToggle)(int state), void (*OnLoad)(), int flags);

/**
 *
 * @brief                   Update some already added text in the quick menu
 * 
 * @param       refID       Refrence ID of the widget
 * @param       Size        Pointer to size of widget
 * @param       Position    Pointer to position of widget
 * @param       Color       Pointer to color of widget
 * @param       Text        Pointer to buffer containing new text
 * @param       OnLoad      Function to be called whenever widget is loaded in quick menu
 * @param       flags       Flags that specify exactly what to update
 * @todo
 * @retval      0 on succes < 0 on error
 * 
*/
EXPORT int QuickMenuRebornUpdateText(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void (*OnLoad)(), int flags);

/**
 *
 * @brief                   Update an already added plane in the quick menu
 * 
 * @param       refID       Refrence ID of the widget
 * @param       Size        Pointer to size of widget
 * @param       Position    Pointer to position of widget
 * @param       Color       Pointer to color of widget
 * @param       OnLoad      Function to be called whenever widget is loaded in quick menu
 * @param       flags       Flags that specify exactly what to update
 * @todo
 * @retval      0 on succes < 0 on error
 * 
*/
EXPORT int QuickMenuRebornUpdatePlane(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, void (*OnLoad)(), int flags);

/**
 *
 * @brief                   Get the last saved value of a checkbox's state
 * 
 * @param       refID       Refrence ID of the widget

 * @todo
 * @retval      Bool: 0 or 1 on succes < 0 on error
 * 
*/
EXPORT int QuickMenuRebornGetCheckBoxState(const char *refID);

#define CONFIG_MGR_ERROR_NOT_EXIST -1
#define CONFIG_MGR_ERROR_COULD_NOT_WRITE_FULL -2
#define CONFIG_MGR_OK 0


#endif