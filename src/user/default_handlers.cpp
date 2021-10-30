#include "default_handlers.h"
#include "config_mgr.h"
#include "linkedList.hpp"
#include "common.hpp"

void defaultCheckBoxSave(const char *refID)
{
    widgetData *data = currentWidgets.GetNode(refID);
    if(data == NULL) return;
    saveCheckBoxState(refID, ((CheckBox *)data->widget)->checked);
}

void defaultSliderSave(const char *refID)
{
    widgetData *data = currentWidgets.GetNode(refID);
    if(data == NULL) return;
    saveSlidebarState(refID, ((ProgressBarTouch *)data->widget)->currentValue);
}

void defaultCheckBoxRecall(const char *refID)
{
    widgetData *data = currentWidgets.GetNode(refID);
    if(data == NULL) return;

    int state = readCheckBoxState(refID);
    if(state == CONFIG_MGR_ERROR_NOT_EXIST) return;

    ((CheckBox *)data->widget)->SetChecked(0, state, 0);
}

void defaultSliderRecall(const char *refID)
{ 
    widgetData *data = currentWidgets.GetNode(refID);
    if(data == NULL) return;

    SceFloat32 state = getSlidebarValue(refID);
    if(state == CONFIG_MGR_ERROR_NOT_EXIST) return;

    ((ProgressBarTouch *)data->widget)->SetProgress(state, 0, 0);
}