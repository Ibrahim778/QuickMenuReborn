#include <stdlib.h>
#include <kernel.h>
#include <libdbg.h>
#include <ctrl.h>
#include "../quickmenureborn/qm_reborn.h"

#define BUTTON_REF_ID "qm_reborn_sample_button"

int count = 0;

BUTTON_HANDLER(onPress)
{
    count++;

    char newText[0x100];
    sceClibSnprintf(newText, 0x100, "You Pressed Me %d Times", count);

    vector4 size = makeWidgetVector4Int(400,75,0,0);
     
    if(count >= 100)
        size.x += 50;
    if(count >= 1000)
        size.x += 50;

    QuickMenuRebornUpdateButton(BUTTON_REF_ID, &size, NULL, NULL, newText, NULL, UPDATE_TEXT | UPDATE_SIZE);
}

int module_start()
{
    vector4 size = makeWidgetVector4Int(200,75,0,0), pos = makeCommonWidgetVector4Int(0);
    widgetColor col = COLOR_WHITE;

    QuickMenuRebornButton(BUTTON_REF_ID, NULL, &size, &pos, &col, "Press Me!", onPress);
    
    return SCE_KERNEL_START_SUCCESS;
}

int module_stop()
{
    removeWidget(BUTTON_REF_ID);
    return SCE_KERNEL_STOP_SUCCESS;
}