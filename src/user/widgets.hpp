#ifndef WIDGETS_H_QM_REBORN
#define WIDGETS_H_QM_REBORN

#include <paf.h>
#include "../quickmenureborn/c_types.h"

using namespace paf;
using namespace widget;

int initWidgets();
int displayWidgets();

#define TYPE_NEEDS_HANDLER(type) (type == button || type == slidebar || type == progressbar_touch || type == check_box)

namespace QMR
{
    Widget *MakeAdvancedWidgetWithID(const char *refId, const char *styleInfo, const char *type, Widget *parent);
    Widget *MakeAdvancedWidgetWithHash(const char *refId, int styleHash, const char *type, Widget *parent);
    Widget *MakeWidgetFromTemplate(const char *id, Widget *targetRoot);

    SceInt32 RegisterWidget(widgetData *dat);
    SceInt32 UnregisterWidget(const char *id);

    SceInt32 SetWidgetColor(const char *refID, float r, float g, float b, float a);
    SceInt32 SetWidgetPosition(const char *refID, float x, float y, float z, float w);
    SceInt32 SetWidgetSize(const char *refID, float x, float y, float z, float w);
    
    SceInt32 RegisterEventHandler(SceInt32 id, widgetData *dat, ECallback Fucntion, void *userDat = SCE_NULL);

};

#endif