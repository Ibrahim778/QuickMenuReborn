#ifndef WIDGETS_H_QM_REBORN
#define WIDGETS_H_QM_REBORN

#include "main.h"
#include "types.h"

#define SCROLL_VIEW_BOX_ID 0x0EE0C8AF
#define ON_PRESS_EVENT_ID 0x10000008

Widget *findWidgetByHash(SceUInt32 hash);
int initWidgets();
Widget *makeWidget(const char *refId, const char *idType, const char *type, Widget *parent);
int displayWidgets();
int editWidget(widgetData *data, int flags);
int registerWidget(widgetData *data);
int unregisterWidget(const char *refId);
int updateWidget(widgetData *data, int flags);
#endif