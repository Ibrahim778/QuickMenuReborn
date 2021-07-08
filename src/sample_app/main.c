#include <kernel.h>
#include <libdbg.h>
#include "../quickmenureborn/qm_reborn.h"

#define BUTTON_REF_ID "sample_button"

int main()
{
    SCE_DBG_LOG_INFO("Hello World!\n");
    SCE_DBG_LOG_INFO("Adding Item to quickmenu\n");
    
    widgetData widget;
    
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, BUTTON_REF_ID, sizeof(widget.refId));

    widget.size = makeWidgetVector4Int(400,75,0,0);
    widget.pos = makeCommonWidgetVector4Int(0);
    widget.type = button;
    widget.col = COLOR_WHITE;

    sceClibMemset(widget.parentRefId, 0, sizeof(widget.parentRefId));
    sceClibStrncpy(widget.parentRefId, NULL_PARENT, sizeof(widget.parentRefId));

    sceClibMemset(widget.data.ButtonData.label, 0, sizeof(widget.data.ButtonData.label));
    sceClibStrncpy(widget.data.ButtonData.label, "Hello, App Exports world!", sizeof(widget.data.ButtonData.label));

    widget.data.ButtonData.onPress = NULL;

    addWidget(&widget);
    
    //QuickMenuRebornButton(BUTTON_REF_ID, NULL_PARENT, makeWidgetVector4Int(400,75,0,0), makeCommonWidgetVector4Int(0), COLOR_YELLOW, "Hi", NULL);
    sceKernelDelayThread(10 * 1000 * 1000);
    removeWidget(&widget);
}