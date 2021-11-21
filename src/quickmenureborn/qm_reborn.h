#ifndef QM_REBORN_H
#define QM_REBORN_H

#define QM_REBORN_INTERNAL

#ifdef __SNC__
#include <scetypes.h>
#else
#include <psp2/types.h>
#endif

#include "c_types.h"

#define BUTTON_HANDLER(name) void name(SceInt32 hash, SceInt32 eventId, void *userDat)
#define ONLOAD_HANDLER(name) void name(const char *id)

#define COLOR_WHITE QuickMenuRebornMakeWidgetColor(1.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_RED QuickMenuRebornMakeWidgetColor(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_GREEN QuickMenuRebornMakeWidgetColor(0.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_BLUE QuickMenuRebornMakeWidgetColor(0.0f, 0.0f, 1.0f, 1.0f)
#define COLOR_YELLOW QuickMenuRebornMakeWidgetColor(1.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_LIGHT_BLUE QuickMenuRebornMakeWidgetColor(0.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_PINK QuickMenuRebornMakeWidgetColor(1.0f, 0, 1.0f, 1.0f)
#define COLOR_YELLOW QuickMenuRebornMakeWidgetColor(1.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_GRAY QuickMenuRebornMakeWidgetColor(0.5f, 0.5f, 0.5f, 0.5f)
#define COLOR_TRANSPARENT QuickMenuRebornMakeWidgetColor(1.0f, 1.0f, 1.0f, 0.0f)

//Size of all the other planes in the quick menu
#define SCE_PLANE_WIDTH 835.0f
#define SCE_SEPARATOR_HEIGHT 20.0f

#define CONFIG_MGR_ERROR_NOT_EXIST -1
#define CONFIG_MGR_ERROR_COULD_NOT_WRITE_FULL -2
#define CONFIG_MGR_OK 0

#define QMR_BUTTON_RELEASE_ID 0x10000008
#define QMR_BUTTON_PRESS_ID 0x10000003

#define QMR_SLIDEBAR_RELEASE_ID 0x10000003


#ifdef __cplusplus
extern "C" {
#endif

vector4 QuickMenuRebornMakeWidgetVector4(float x, float y, float z, float w);
widgetColor QuickMenuRebornMakeWidgetColor(float r, float g, float b, float a);

widgetData *QuickMenuRebornRegisterWidget(const char *id, const char *parentId, QMRWidgetType type);
int QuickMenuRebornRegisterWidgetFromData(widgetData *widgetInfo);
int QuickMenuRebornUnregisterWidget(const char *id);
int QuickMenuRebornRegisterEventHanlder(const char *widgetID, SceInt32 EventID, ECallback Fucntion, void *userDat);

int QuickMenuRebornSeparator(const char *id, float height);
int QuickMenuRebornRemoveSeparator(const char *id);

int QuickMenuRebornSetWidgetSize(const char *refID, float x, float y, float z, float w);
int QuickMenuRebornSetWidgetPosition(const char *refID, float x, float y, float z, float w);
int QuickMenuRebornSetWidgetColor(const char *refID, float r, float g, float b, float a);
int QuickMenuRebornSetWidgetLabel(const char *refID, const char *label);

float QuickMenuRebornGetSlidebarValue(const char *refID);
int QuickMenuRebornGetCheckboxValue(const char *refID);

int QuickMenuRebornAssignRecallHandler(VoidCallback callback, const char *refID);
int QuickMenuRebornAssignSaveHandler(VoidCallback callback, const char *refID);
int QuickMenuRebornAssignOnLoadHandler(VoidCallback callback, const char *refID);

int QuickMenuRebornAssignDefaultCheckBoxSave(const char *refID);
int QuickMenuRebornAssignDefaultCheckBoxRecall(const char *refID);

int QuickMenuRebornAssignDefaultSliderSave(const char *refID);
int QuickMenuRebornAssignDefaultSliderRecall(const char *refID);

#ifdef __cplusplus
}
#endif

#endif