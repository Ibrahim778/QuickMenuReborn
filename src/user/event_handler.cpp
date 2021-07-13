#include "main.h"
#include "widgets.h"

extern "C" int sceRegMgrSetKeyInt(const char *category, const char *name, int value);

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
                int ret = sceRegMgrSetKeyInt(REG_CONFIG_DIR, widget->refId, ((CheckBox *)self)->checked);
                sceClibPrintf("Set reg res: 0x%X\n", ret);
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