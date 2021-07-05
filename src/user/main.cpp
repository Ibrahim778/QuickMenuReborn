#include "main.h"
#include "types.h"
#include "widgets.h"
#include "../quickmenureborn/qm_reborn.h"
#include "event_handler.cpp"

SceUID mainThreadID = -1;

int dispalyed = 0;

/*
SceVoid leakTestTask(void)
{
    Allocator *glAlloc = Allocator::GetGlobalAllocator();
    SceInt32 sz = glAlloc->GetFreeSize();
    
    sceClibPrintf("[EMPVA_DEBUG] Free heap memory: %u\n", sz);
}
*/

int impose_thread(SceSize, void *)
{
    qm_reborn_eztext("mytext", NULL, makeWidgetVector4(300.0f,75.0f,0.0f,0.0f), makeWidgetVector4(0.0f,0.0f,0.0f,0.0f), makeWidgetColor(1.0f,1.0f,1.0f,1.0f), (char *)"Hi", 1);
    SceAppMgrAppState state;
    while (1)
    {
        sceAppMgrGetAppState(&state);
        if(state.isSystemUiOverlaid)
        {
            if(!dispalyed)
            {
                int ret;
                ret = initWidgets();
                FAILTHREAD_IF(ret < 0);

                //Delay a little to make sure it displays at the end
                sceKernelDelayThread(1000);

                //leakTestTask();

                displayWidgets();
                dispalyed = 1;
            }
        }
        else dispalyed = 0;
        //Prevent stalling thread
        sceKernelDelayThread(10 * 1000);
    }
    
    //return sceKernelExitDeleteThread(0);
}

extern "C"
{
    int module_start(SceSize, void *)
    {
        QMEventHandler eh;
        mainThreadID = sceKernelCreateThread("quickmenureborn", impose_thread, 250, 0x10000, 0, 0, NULL);
        sceKernelStartThread(mainThreadID, 0, NULL);
        return SCE_KERNEL_START_SUCCESS;
    }

    int module_stop()
    {
        if(mainThreadID > 0)
            sceKernelDeleteThread(mainThreadID);
        return SCE_KERNEL_STOP_SUCCESS;
    }
} 