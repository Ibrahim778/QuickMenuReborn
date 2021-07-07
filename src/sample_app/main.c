#include <kernel.h>
#include <libdbg.h>
#include "../quickmenureborn/qm_reborn.h"

#define TEXT_REF_ID "qm_reborn_samples_sample_text"

int main()
{
    SCE_DBG_LOG_INFO("Hello World!\n");
    SCE_DBG_LOG_INFO("Adding Item to quickmenu\n");

    widgetData dat;
    dat.refId = "mytext";
    addWidget(dat);

    sceKernelDelayThread(10 * 1000 * 1000);
    removeWidget("mytext");
}