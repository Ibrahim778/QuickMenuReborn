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
#define NULL_PARENT "NULL"

#define NULL_SET(str, src) do { sce_paf_memset(&str, 0, sizeof(str)); sceClibStrncpy((char *)&str, src, sizeof(str)); }while(0)

EXPORT
int addWidget(widgetData *data);

EXPORT
int updateWidget(widgetData *data, int flags);

EXPORT
int removeWidget(widgetData *data);

EXPORT
widgetColor makeWidgetColor(float r, float g, float b, float a);
EXPORT
vector4 makeWidgetVector4(float x, float y, float z, float w);

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

int QuickMenuRebornButton(const char *refID, const char *parentRefID, vector4 Size, vector4 Position, widgetColor Color, const char *Text, void(*OnPress)(void));
int QuickMenuRebornCheckBox(const char *refID, const char *parentRefID, vector4 Size, vector4 Position, widgetColor Color, void(*OnToggle)(int state));
int QuickMenuRebornText(const char *refID, const char *parentRefID, vector4 Size, vector4 Position, widgetColor Color, const char *Text);
int QuickMenuRebornPlane(const char *refID, const char *parentRefID, vector4 Size, vector4 Position, widgetColor Color);
int QuickMenuRebornSeparator(const char *refID);

int QuickMenuRebornUpdateButton(const char *refID, vector4 Size, vector4 Position, widgetColor Color, const char *Text, void(*OnPress)(void), int flags);
int QuickMenuRebornUpdateCheckBox(const char *refID, vector4 Size, vector4 Position, widgetColor Color, void(*OnToggle)(int state), int flags);
int QuickMenuRebornUpdateText(const char *refID, vector4 Size, vector4 Position, widgetColor Color, const char *Text, int flags);
int QuickMenuRebornUpdatePlane(const char *refID, vector4 Size, vector4 Position, widgetColor Color, int flags);

#endif