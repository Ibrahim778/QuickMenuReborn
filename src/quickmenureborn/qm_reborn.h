#ifndef QM_REBORN_H
#define QM_REBORN_H

#include "c_types.h"

void *sce_paf_memset(void *ptr, int value, SceSize num);
char *sceClibStrncpy(char *dst, const char *src, SceSize len);
int sceClibSnprintf(char *dst, SceSize dst_max_size, const char *fmt, ...);

#define EXPORT extern 

#ifndef QM_REBORN_INTERNAL
#undef EXPORT
#define EXPORT
#endif

#define CHECKBOX_HANDLER(name) void name(int checked)
#define BUTTON_HANDLER(name) void name()
#define ONLOAD_HANDLER(name) void name()

#define NULL_SET(str, src) do { sce_paf_memset(&str, 0, sizeof(str)); sceClibStrncpy((char *)&str, src, sizeof(str)); }while(0)

EXPORT
int addWidget(widgetData *data);

EXPORT
int updateWidget(widgetData *data, int flags);

EXPORT
int removeWidget(const char *data);

EXPORT
int openQuickMenu();

EXPORT
widgetColor makeWidgetColor(float r, float g, float b, float a);
EXPORT
vector4 makeWidgetVector4(float x, float y, float z, float w);

EXPORT
int QuickMenuRebornAddAdvancedWidget(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Type, const char *idType, void (*OnLoad)());

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

//Size of all the other planes in the quickmenu
#define SCE_PLANE_WIDTH 825.0f

EXPORT
int QuickMenuRebornButton(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void (*OnLoad)(), void(*OnPress)(void));
EXPORT
int QuickMenuRebornCheckBox(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, void (*OnLoad)(), void(*OnToggle)(int state));
EXPORT
int QuickMenuRebornText(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void (*OnLoad)());
EXPORT
int QuickMenuRebornPlane(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, void (*OnLoad)());
EXPORT
int QuickMenuRebornSeparator(const char *refID);

EXPORT
int QuickMenuRebornUpdateButton(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void (*OnLoad)(), void(*OnPress)(void), int flags);
EXPORT
int QuickMenuRebornUpdateCheckBox(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, void (*OnLoad)(), void(*OnToggle)(int state), int flags);
EXPORT
int QuickMenuRebornUpdateText(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void (*OnLoad)(), int flags);
EXPORT
int QuickMenuRebornUpdatePlane(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, void (*OnLoad)(), int flags);

#endif