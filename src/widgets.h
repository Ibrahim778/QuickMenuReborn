#ifndef WIDGETS_H_QM_REBORN
#define WIDGETS_H_QM_REBORN

#include "main.h"
#include "types.h"

#define SCROLL_VIEW_BOX_ID 0x0EE0C8AF
#define ON_PRESS_EVENT_ID 0x10000008

#define CHECKBOX_HANDLER(name) void name(int checked)
#define BUTTON_HANDLER(name) void name()

static widget::Widget *(*getImposeRoot)();
static Plugin *imposePlugin;
static widget::Widget *powerRoot;
static Widget *main_plane;


Widget *findWidgetByHash(SceUInt32 hash);
int initWidgets();
Widget *makeWidget(char *refId, char *idType, char *type, Widget *parent);
int displayWidgets();

#endif