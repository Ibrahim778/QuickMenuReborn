#include "main.h"
#include "widgets.h"

class QMEventHandler : public Widget::EventCallback
{
public:
    QMEventHandler()
    {
        eventHandler = onGet;
    }

    static void onGet(SceInt32 eventId, Widget *self, SceInt32, ScePVoid pUserData)
    {
        widgetData *widget = (widgetData *)pUserData;
        switch (widget->type)
        {
            case button:
            {
                if(widget->data.ButtonData.onPress != NULL) widget->data.ButtonData.onPress();
                break;
            }
            
            case check_box:
            {
                if(widget->data.CheckBoxData.OnToggle != NULL) widget->data.CheckBoxData.OnToggle(((CheckBox *)self)->checked);    
                break;
            }

            default:
            {
                break;
            }
        }
    }
};