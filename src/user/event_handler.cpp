#include "main.h"
#include "widgets.h"

extern "C" void handleButton(void(*button)(void));

class QMEventHandler : public Widget::EventCallback
{
public:
    QMEventHandler()
    {
        eventHandler = onGet;
    }

    static void onGet(SceInt32 , Widget *self, SceInt32, ScePVoid puserData)
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