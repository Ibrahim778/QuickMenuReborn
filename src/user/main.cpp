
#include "types.h"
#include "widgets.h"

#include "../quickmenureborn/qm_reborn.h"
#include "main.h"

SceUID mainThreadID = SCE_UID_INVALID_UID;
SceUID exportThreadID = SCE_UID_INVALID_UID;

bool exportEnd = false;
bool mainEnd = false;

int dispalyed = 0;

#define INTERNAL_SPACER_ID "qm_reborn_internal_spacer"

#ifdef DEBUG
SceVoid leakTestTask(void)
{
    Allocator *glAlloc = Allocator::GetGlobalAllocator();
    SceInt32 sz = glAlloc->GetFreeSize();
    
    SCE_DBG_LOG_INFO("Free heap memory: %d\n", sz);
}
#endif

//Add an invisible spacer after accesability widgets, just to make things look a bit cleaner
int addInitialSpacer()
{
    widgetData widget;
    sce_paf_memset(&widget, 0, sizeof(widget));

    widget.type = plane;

    //Need to set everything manually
    widget.col.r = 1.0f;
    widget.col.g = 1.0f;
    widget.col.b = 1.0f;
    widget.col.a = 0.0f;

    widget.size.x = 825.0f;
    widget.size.y = 20.0f;
    widget.size.z = 0.0f;
    widget.size.w = 0.0f;
    
    sce_paf_memcpy(widget.refId, INTERNAL_SPACER_ID, sizeof(widget.refId));

    return registerWidget(&widget);
}

int export_thread(SceSize, void *)
{
    SceUID pipeID = sceKernelCreateMsgPipe("quickmenureborn_exports_pipe", SCE_KERNEL_MSG_PIPE_TYPE_USER_MAIN, SCE_KERNEL_ATTR_OPENABLE, SCE_KERNEL_4KiB, NULL);
    if(pipeID < 0)
    {
        exportThreadID = SCE_UID_INVALID_UID;
        return sceKernelExitDeleteThread(0);
    }
    while (1)
    {
        if(exportEnd) break;

        exportPacket data;
        SceSize size = 0;

        int recRes = sceKernelReceiveMsgPipe(pipeID, &data, sizeof(data), SCE_KERNEL_MSG_PIPE_MODE_WAIT | SCE_KERNEL_MSG_PIPE_MODE_FULL, &size, NULL);
        if(recRes != SCE_OK) 
            SCE_DBG_LOG_INFO("Error pipe recieve function returned error code : 0x%X\n", recRes);
        else if(size > 0) 
        {
            #ifdef DEBUG
            SCE_DBG_LOG_INFO("Got call\n");
            #endif
            switch (data.type)
            {
                case update_widget:
                {
                    updateWidget(&data.data, data.updateFlags);
                    break;
                }
                case register_widget:
                {
                    #ifdef DEBUG
                    SCE_DBG_LOG_INFO("adding widget with refID %s\n", data.data.refId);
                    #endif
                    registerWidget(&data.data);
                    break;
                }
                case unregister_widget:
                {
                    unregisterWidget(data.data.refId);
                    break;
                }
                case open_quickmenu:
                {
                    openQuickMenu();
                    break;
                }
                default:
                    break;
            }
        }
    }

    return sceKernelExitDeleteThread(0);
}

int impose_thread(SceSize, void *)
{
    //Delay to let shell load properly
    sceKernelDelayThread(4 * 1000 * 1000);
    
    addInitialSpacer();

    SceAppMgrAppState state;
    while (1)
    {

        if(mainEnd) break;

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

    return sceKernelExitDeleteThread(0);
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