#include "event_handler.hpp"

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
                sceClibPrintf("Setting checkbox state to %d\n", ((CheckBox *)self)->checked);
                currentWidgets.update_checkbox_status(widget->refId, ((CheckBox *)self)->checked);
                if(widget->data.CheckBoxData.OnToggle != NULL) widget->data.CheckBoxData.OnToggle(((CheckBox *)self)->checked);    
                break;
            }

            default:
            {
                break;
            }
        }
    }
