#include "main.h"
#include "types.h"
#include "widgets.h"
#include "../quickmenureborn/qm_reborn.h"
#include "event_handler.cpp"

SceUID mainThreadID = -1;

int dispalyed = 0;

int impose_thread(SceSize args, void *argp)
{
    qm_reborn_eztext("mytext", NULL, makeWidgetVector4(300,75,0,0), makeWidgetVector4(0,0,0,0), makeWidgetColor(1,1,1,1), "Hi", 1);
    SceAppMgrAppState state;
    while (1)
    {
        _sceAppMgrGetAppState(&state, sizeof(state), 0);
        if(state.isSystemUiOverlaid)
        {
            if(!dispalyed)
            {
                int ret;
                ret = initWidgets();
                FAILTHREAD_IF(ret < 0);

                //Delay a little to make sure it displays at the end
                sceKernelDelayThread(1000);
                displayWidgets();
                dispalyed = 1;
            }
        }
        else dispalyed = 0;
        //Prevent stalling thread
        sceKernelDelayThread(10 * 1000);
    }
    
    return sceKernelExitDeleteThread(0);
}

extern "C"
{
    int _start(SceSize args, void *argp) __attribute__((weak, alias("module_start")));
    int module_start(SceSize args, void *argp)
    {
        QMEventHandler eh;
        mainThreadID = sceKernelCreateThread("quickmenureborn", impose_thread, 250, 0x10000, 0, 0, NULL);
        int start = sceKernelStartThread(mainThreadID, 0, NULL);
        return SCE_KERNEL_START_SUCCESS;
    }

    int module_stop()
    {
        if(mainThreadID > 0)
            sceKernelDeleteThread(mainThreadID);
        return SCE_KERNEL_STOP_SUCCESS;
    }
} 