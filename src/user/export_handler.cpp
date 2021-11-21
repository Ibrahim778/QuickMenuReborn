#define QM_REBORN_INTERNAL
#include "../quickmenureborn/qm_reborn.h"
#include <kernel.h>
#include <libdbg.h>
#include "main.h"
#include "widgets.hpp"
#include "config_mgr.h"
#include "linkedList.hpp"
#include "common.hpp"
#include "default_handlers.h"

widgetColor QuickMenuRebornMakeWidgetColor(float r, float g, float b, float a)
{
    widgetColor c;
    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;

    return c;
}

vector4 QuickMenuRebornMakeWidgetVector4(float x, float y, float z, float w)
{
    vector4 v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;

    return v;
}

int QuickMenuRebornRegisterWidgetFromData(widgetData *widgetInfo)
{
    QMR::RegisterWidget(widgetInfo);
}

widgetData *QuickMenuRebornRegisterWidget(const char *id, const char *parentId, QMRWidgetType type)
{
    widgetData dat;
    sce_paf_memset(&dat, 0, sizeof(dat));

    sce_paf_strncpy(dat.refId, id, sizeof(dat.refId));

    if(parentId != NULL)
        sce_paf_strncpy(dat.parentRefId, parentId, sizeof(dat.parentRefId));
    
    dat.hasParent = parentId != NULL;

    dat.type = type;

    return QMR::RegisterWidget(&dat);
}

int QuickMenuRebornUnregisterWidget(const char *id)
{
    if(widgetsDisplayed())
    {
        Widget *w = Utils::FindWidget(Utils::GetHashById(id));
        if(w != NULL) w->PlayAnimationReverse(0, Widget::Animation_Reset);
    }
    QMR::UnregisterWidget(id);
}

int QuickMenuRebornRegisterEventHanlder(const char *widgetID, SceInt32 EventID, ECallback Function, void *userDat)
{
    QMR::RegisterEventHandler(EventID, currentWidgets.GetNode(widgetID), Function, userDat);
}

int QuickMenuRebornSetWidgetSize(const char *refID, float x, float y, float z, float w)
{
    widgetData *data = currentWidgets.GetNode(refID);
    if(data == NULL) return;

    data->size.x = x;
    data->size.y = y;
    data->size.z = z;
    data->size.w = w;

    if(widgetsDisplayed())
    {
        ((Widget *)data->widget)->SetSize((SceFVector4 *)&data->size);
    }
}

int QuickMenuRebornSetWidgetPosition(const char *refID, float x, float y, float z, float w)
{
    widgetData *data = currentWidgets.GetNode(refID);
    if(data == NULL) return;

    data->pos.x = x;
    data->pos.y = y;
    data->pos.z = z;
    data->pos.w = w;

    if(widgetsDisplayed())
        Utils::SetWidgetPosition((Widget *)data->widget, x, y, z, w);
}

int QuickMenuRebornSetWidgetColor(const char *refID, float r, float g, float b, float a)
{
    widgetData *data = currentWidgets.GetNode(refID);
    if(data == NULL) return;

    data->col.r = r;
    data->col.g = g;
    data->col.b = b;
    data->col.a = a;

    if(widgetsDisplayed())
        Utils::SetWidgetColor((Widget *)data->widget, r, g, b, a);
    
}

int QuickMenuRebornSetWidgetLabel(const char *refID, const char *label)
{
    widgetData *data = currentWidgets.GetNode(refID);
    if(data == NULL) return;
    sce_paf_memset(data->label, 0, sizeof(data->label));
    sce_paf_strncpy(data->label, label, sizeof(data->label));

    if(widgetsDisplayed())
    {
        WString wstr;
        
        WString::CharToNewWString(label, &wstr);
        
        ((Widget *)data->widget)->SetLabel(&wstr);
    }
}

float QuickMenuRebornGetSlidebarValue(const char *refID)
{
    widgetData *dat = currentWidgets.GetNode(refID);
    if(dat == NULL) return -1;

    return ((ProgressBarTouch *)dat->widget)->currentValue;
}

int QuickMenuRebornGetCheckboxValue(const char *refID)
{
    widgetData *dat = currentWidgets.GetNode(refID);
    if(dat == NULL) return -1;

    return ((CheckBox *)dat->widget)->checked;
}

int QuickMenuRebornSeparator(const char *id, float height)
{
    widgetData dat;
    sce_paf_memset(&dat, 0, sizeof(dat));

    sce_paf_snprintf(dat.refId, sizeof(dat.refId), "qmr_sep_%s_internal", id);

    dat.hasParent = false;
    dat.isAdvanced = true;
    
    dat.size.x = SCE_PLANE_WIDTH;
    dat.size.y = height;

    dat.col = COLOR_WHITE;

    dat.advancedData.useHash = true;

    sce_paf_strncpy(dat.advancedData.type, "plane", 5);
    
    dat.advancedData.StyleInfo.hash = 0x26C7781E;

    QMR::RegisterWidget(&dat);
}

int QuickMenuRebornRemoveSeparator(const char *id)
{
    char rID[0x100];
    sce_paf_memset(rID, 0, sizeof(rID));

    sce_paf_snprintf(rID, sizeof(rID), "qmr_sep_%s_internal", id);

    QMR::UnregisterWidget(rID);

}

int QuickMenuRebornAssignRecallHandler(VoidCallback callback, const char *refID)
{
    widgetData *data = currentWidgets.GetNode(refID);
    if(data == NULL) return;

    data->OnRecall = callback;
}

int QuickMenuRebornAssignSaveHandler(VoidCallback callback, const char *refID)
{
    widgetData *data = currentWidgets.GetNode(refID);
    if(data == NULL) return;

    data->OnRecall = callback;
}

int QuickMenuRebornAssignOnLoadHandler(VoidCallback callback, const char *refID)
{
    widgetData *data = currentWidgets.GetNode(refID);
    if(data == NULL) return;

    data->OnLoad = callback;
}

int QuickMenuRebornAssignDefaultCheckBoxSave(const char *refID)
{
    widgetData *data = currentWidgets.GetNode(refID);
    if(data == NULL) return;

    data->OnSave = defaultCheckBoxSave;
}

int QuickMenuRebornAssignDefaultCheckBoxRecall(const char *refID)
{
    widgetData *data = currentWidgets.GetNode(refID);
    if(data == NULL) return;

    data->OnRecall = defaultCheckBoxRecall;
}

int QuickMenuRebornAssignDefaultSliderSave(const char *refID)
{
    widgetData *data = currentWidgets.GetNode(refID);
    if(data == NULL) return;

    data->OnSave = defaultSliderSave;
}

int QuickMenuRebornAssignDefaultSliderRecall(const char *refID)
{
    widgetData *data = currentWidgets.GetNode(refID);
    if(data == NULL) return;

    data->OnRecall = defaultSliderRecall;
}

int QuickMenuRebornSetSlidebarValue(const char *refID, float val)
{
    if(!widgetsDisplayed()) return -1;

    widgetData *data = currentWidgets.GetNode(refID);
    if(data == NULL) return;

    ((ProgressBarTouch *)data->widget)->SetProgress(val, 0, 0);
}

int QuickMenuRebornSaveSlidebarValue(const char *refID, float val)
{
    saveSlidebarState(refID, val);
}

int QuickMenuRebornSetCheckBoxState(const char *refID, int state)
{
    if(!widgetsDisplayed()) return -1;

    widgetData *data = currentWidgets.GetNode(refID);
    if(data == NULL) return;

    ((CheckBox *)data->widget)->SetChecked(state, 0, 0);

}

int QuickMenuRebornSaveCheckBoxState(const char *refID, int state)
{
    saveCheckBoxState(refID, state);
}