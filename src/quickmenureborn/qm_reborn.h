#ifndef QM_REBORN_H
#define QM_REBORN_H

#define QM_REBORN_INTERNAL

#include <scetypes.h>
#include "c_types.h"

#define CHECKBOX_HANDLER(name) void name(int checked)
#define BUTTON_HANDLER(name) void name()
#define ONLOAD_HANDLER(name) void name()

#ifdef __cplusplus 
extern "C" 
#else 
extern 
#endif 
int addWidget(widgetData *data);

#ifdef __cplusplus 
extern "C" 
#else 
extern 
#endif 
int updateWidget(widgetData *data, int flags);

#ifdef __cplusplus 
extern "C" 
#else 
extern 
#endif 
int removeWidget(const char *data);

#ifdef __cplusplus 
extern "C" 
#else 
extern 
#endif 
int removeSeparator(const char *refID);


#ifdef __cplusplus 
extern "C" 
#else 
extern 
#endif 
widgetColor makeWidgetColor(float r, float g, float b, float a);

#ifdef __cplusplus 
extern "C" 
#else 
extern 
#endif 
vector4 makeWidgetVector4(float x, float y, float z, float w);


#ifdef __cplusplus 
extern "C" 
#else 
extern 
#endif 
int QuickMenuRebornAddAdvancedWidget(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Type, const char *style, void (*OnLoad)());

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

//Size of all the other planes in the quickmenu (as close as I could get)
#define SCE_PLANE_WIDTH 825.0f


#ifdef __cplusplus 
extern "C" 
#else 
extern 
#endif 
int QuickMenuRebornButton(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void (*OnLoad)(), void(*OnPress)(void));

/**
 *
 * @brief                   Add a checkbox to the quickmenu
 * 
 * @param       refID       Refrence ID of the widget
 * @param       parentRefID Refrence ID of the parent widget (pass NULL if none)
 * @param       Size        Size of widget
 * @param       Position    Position of widget
 * @param       Color       Color of widget
 * @param       OnLoad      Function to be called whenever widget is loaded in quickmenu
 * @param       OnToggle    Function to be called whenever the checkbox is pressed (access the current state with the variable state)
 * @param       state       Initial state of Check Box when spawned, ON = On, OFF = Off, PREV_STATE = previous saved state (saved on reboot / poweroff)
 * @todo
 * @retval      0 on succes < 0 on error
 * 
*/
#ifdef __cplusplus 
extern "C" 
#else 
extern 
#endif 
int QuickMenuRebornCheckBox(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, void (*OnLoad)(), void(*OnToggle)(int state), CheckBoxState state);

#ifdef __cplusplus 
extern "C" 
#else 
extern 
#endif 
int QuickMenuRebornText(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void (*OnLoad)());

#ifdef __cplusplus 
extern "C" 
#else 
extern 
#endif 
int QuickMenuRebornPlane(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, void (*OnLoad)());

#ifdef __cplusplus 
extern "C" 
#else 
extern 
#endif 
int QuickMenuRebornSeparator(const char *refID);


#ifdef __cplusplus 
extern "C" 
#else 
extern 
#endif 
int QuickMenuRebornUpdateButton(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void(*OnPress)(void), void (*OnLoad)(), int flags);

#ifdef __cplusplus 
extern "C" 
#else 
extern 
#endif 
int QuickMenuRebornUpdateCheckBox(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, void(*OnToggle)(int state), void (*OnLoad)(), int flags);

#ifdef __cplusplus 
extern "C" 
#else 
extern 
#endif 
int QuickMenuRebornUpdateText(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void (*OnLoad)(), int flags);

#ifdef __cplusplus 
extern "C" 
#else 
extern
#endif 
int QuickMenuRebornUpdatePlane(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, void (*OnLoad)(), int flags);

#define QM_REBORN_SEMA_NAME "qm_reborn_semaphore"

#endif