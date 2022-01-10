#include "event_handler.hpp"
#include "config_mgr.h"
#include "common.hpp"

extern WidgetList currentWidgets;

QMREventHandler::QMREventHandler()
{
    eventHandler = onGet;
}

void QMREventHandler::onGet(SceInt32 eventId, Widget *self, SceInt32, ScePVoid puserData)
{
    widgetData *widgetInfo = (widgetData *)puserData;

    if(puserData == NULL || self == NULL || widgetInfo == NULL || widgetInfo->CallbackNum == 0 || widgetInfo->Callbacks == NULL)
        return;
    
    for (int i = 0; i < widgetInfo->CallbackNum; i++)
    {
        if(widgetInfo->Callbacks[i].function != NULL && widgetInfo->Callbacks[i].id == eventId)
            widgetInfo->Callbacks[i].function(widgetInfo->refId, self->hash, eventId, widgetInfo->Callbacks[i].userDat);
    }   

    if(widgetInfo->OnSave != NULL)
        widgetInfo->OnSave(widgetInfo->refId);
}