#include <kernel.h>
#include <libdbg.h>
#include "../quickmenureborn/qm_reborn.h"

#define BUTTON_REF_ID "qm_reborn_sample_button"

int module_start()
{
    SCE_DBG_LOG_INFO("Hello World!\n");
    int ret = QuickMenuRebornButton(BUTTON_REF_ID, NULL, makeWidgetVector4Int(400,75,0,0), makeCommonWidgetVector4Int(0), COLOR_YELLOW, "Hi", NULL);
    
    SCE_DBG_LOG_INFO("Got result %d", ret);
    return SCE_KERNEL_START_SUCCESS;
}

int module_stop()
{
    removeWidget(BUTTON_REF_ID);
    return SCE_KERNEL_STOP_SUCCESS;
}