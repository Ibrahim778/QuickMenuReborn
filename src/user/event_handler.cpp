#include "event_handler.hpp"
#include "config_mgr.h"

extern linked_list currentWidgets;

QMEventHandler::QMEventHandler()
{
    eventHandler = onGet;
}

void QMEventHandler::onGet(SceInt32 , Widget *self, SceInt32, ScePVoid puserData)
{
    widgetData *widget = (widgetData *)puserData;
    switch (widget->type)
    {
        case button:
        {
            if(widget->data.ButtonData.onPress != NULL) widget->data.ButtonData.onPress();
            break;
        }
            
        case check_box:
        {
            currentWidgets.update_checkbox_status(widget->refId, ((CheckBox *)self)->checked ? CHECKBOX_ON : CHECKBOX_OFF);
            if(widget->data.CheckBoxData.OnToggle != NULL) widget->data.CheckBoxData.OnToggle(((CheckBox *)self)->checked);    
            break;
        }

        case slidebar:
        {
            if(widget->data.SlidebarData.OnChange != NULL) widget->data.SlidebarData.OnChange();
            break;
        }

        default:
        {
            break;
        }
    }
}
