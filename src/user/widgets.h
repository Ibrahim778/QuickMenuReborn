#ifndef WIDGETS_H_QM_REBORN
#define WIDGETS_H_QM_REBORN

#define SCROLL_VIEW_BOX_ID 0x0EE0C8AF
#define ON_PRESS_EVENT_ID 0x10000008

Widget *findWidgetByHash(SceUInt32 hash);
int initWidgets();
Widget *makeWidget(const char *refId, const char *styleInfo, const char *type, Widget *parent);
int displayWidgets();
int openQuickMenu();
int editWidget(widgetData *data, int flags);
int registerWidget(widgetData *data);
int unregisterWidget(const char *refId);
int update_Widget(widgetData *data, int flags);
int spawn(widgetData *widget, int flags);
void dummyprint(const char *fmt, ...);
#endif