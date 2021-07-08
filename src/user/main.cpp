#include "main.h"
#include "types.h"
#include "widgets.h"
#include "../quickmenureborn/qm_reborn.h"
#include "event_handler.cpp"

SceUID mainThreadID = SCE_UID_INVALID_UID;
SceUID exportThreadID = SCE_UID_INVALID_UID;

bool exportEnd = false;
bool mainEnd = false;

int dispalyed = 0;

#ifdef DEBUG
SceVoid leakTestTask(void)
{
    Allocator *glAlloc = Allocator::GetGlobalAllocator();
    SceInt32 sz = glAlloc->GetFreeSize();
    
    SCE_DBG_LOG_INFO("Free heap memory: %d\n", sz);
}
#endif

int export_thread(SceSize, void *)
{
    SceUID pipeID = sceKernelCreateMsgPipe("quickmenureborn_exports_pipe", SCE_KERNEL_MSG_PIPE_TYPE_USER_MAIN, SCE_KERNEL_ATTR_OPENABLE, SCE_KERNEL_4KiB, NULL);
    if(pipeID < 0)
    {
        exportThreadID = SCE_UID_INVALID_UID;
        sceKernelExitDeleteThread(0);
    }
    while (1)
    {
        if(exportEnd) sceKernelExitDeleteThread(0);

        exportPacket data;
        SceSize size = 0;

        int recRes = sceKernelReceiveMsgPipe(pipeID, &data, sizeof(data), SCE_KERNEL_MSG_PIPE_MODE_WAIT | SCE_KERNEL_MSG_PIPE_MODE_FULL, &size, NULL);
        if(recRes != SCE_OK) SCE_DBG_LOG_INFO("Error function returned error code : 0x%X\n", recRes);
        else if(size > 0) 
        {
            SCE_DBG_LOG_INFO("Got call\n");
            switch (data.type)
            {
                case update_widget:
                {
                    updateWidget(&data.data, data.updateFlags);
                    break;
                }
                case register_widget:
                {
                    SCE_DBG_LOG_INFO("adding widget with refID %s\n", data.data.refId);
                    registerWidget(&data.data);
                    break;
                }
                case unregister_widget:
                {
                    unregisterWidget((char *)&data.data.refId);
                    break;
                }
                default:
                    break;
            }
        }
    }
}

int impose_thread(SceSize, void *)
{
    sceKernelDelayThread(4 * 1000 * 1000);
    SceAppMgrAppState state;
    while (1)
    {

        if(mainEnd) sceKernelExitDeleteThread(0);

        sceAppMgrGetAppState(&state);
        if(state.isSystemUiOverlaid)
        {
            if(!dispalyed)
            {
                int ret;
                ret = initWidgets();
                if(ret >= 0)
                {
                    //Delay a little to make sure it displays at the end
                    sceKernelDelayThread(1000);
#ifdef DEBUG
                    leakTestTask();
#endif
                    displayWidgets();
                    dispalyed = 1;
                }
            }
        }
        else dispalyed = 0;
        //Prevent stalling thread
        sceKernelDelayThread(10 * 1000);
    }    
}

extern "C"
{
    int module_start(SceSize, void *)
    {
        mainThreadID = sceKernelCreateThread("quickmenureborn", impose_thread, 250, SCE_KERNEL_4KiB, 0, 0, NULL);
        if(sceKernelStartThread(mainThreadID, 0, NULL) < 0) return SCE_KERNEL_START_NO_RESIDENT;
        
        exportThreadID = sceKernelCreateThread("quickmenureborn_export_thread", export_thread, 251, SCE_KERNEL_4KiB, 0, 0, NULL);
        if(sceKernelStartThread(exportThreadID, 0, NULL) < 0) return SCE_KERNEL_START_NO_RESIDENT;
        
        return SCE_KERNEL_START_SUCCESS;
    }

    int module_stop()
    {
        if(mainThreadID > 0)
        {
            mainEnd = true;
            sceKernelWaitThreadEnd(mainThreadID, NULL, NULL);
        }
        if(exportThreadID)
        {
            exportEnd = true;
            sceKernelWaitThreadEnd(exportThreadID, NULL, NULL);
        }    

        return SCE_KERNEL_STOP_SUCCESS;
    }
} 