#ifndef QM_REBORN_COMMON_HPP
#define QM_REBORN_COMMON_HPP

#include "linkedList.hpp"
#include <paf.h>
using namespace paf;
using namespace ui;

extern Widget *(*getImposeRoot)();
extern Plugin *imposePlugin;
extern ScePVoid powerRoot;
extern Box *scrollBox;
extern Widget *exitButton;
extern Plugin *QuickMenuRebornPlugin;
extern WidgetList currentWidgets;
extern TexList currTextures;
extern bool displayed;

#define SCROLL_VIEW_BOX_ID 0x0EE0C8AF

#define BUTTON_TEMPLATE_ID "qmr_button_template"
#define CHECKBOX_TEMPLATE_ID "qmr_checkbox_template"
#define TEXT_TEMPLATE_ID "qmr_text_template"
#define PLANE_TEMPLATE_ID "qmr_plane_template"
#define SLIDEBAR_TEMPLATE_ID "qmr_slidebar_template"
#define PROGRESSBAR_TOUCH_TEMPLATE_ID "qmr_progressbar_touch_template"
#define BUSYINDICATOR_TEMPLATE_ID "qmr_busyindicator_template"

#endif