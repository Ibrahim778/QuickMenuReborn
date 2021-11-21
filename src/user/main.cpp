
#include "main.h"
#include "utils.hpp"
#include "widgets.hpp"
#include "../quickmenureborn/qm_reborn.h"
#include "common.hpp"

SceUID mainThreadID = SCE_UID_INVALID_UID;
bool mainEnd = false;
bool displayed = false;

#ifdef _DEBUG
SceVoid leakTestTask(void)
{
    Allocator *glAlloc = Allocator::GetGlobalAllocator();
    SceInt32 sz = glAlloc->GetFreeSize();
    
    SCE_DBG_LOG_INFO("Free heap memory: %d\n", sz);
}
#endif

SceInt32 VblankCallback(SceUID notifyId, SceInt32 notifyCount, SceInt32 notifyArg, void* pCommon) 
{
    SceAppMgrAppState state;

    sceAppMgrGetAppState(&state);
    if(state.isSystemUiOverlaid)
    {
        if(!displayed)
        {
            if(initWidgets() >= 0)
            {

                //Delay to make sure it displays it at the end
                sceKernelDelayThread(1000);

#ifdef _DEBUG
                leakTestTask();
#endif
                displayed = true;
                displayWidgets();
            
            }
        }
    }
    else displayed = false;

    return 0;
}

int impose_thread(SceSize, void *)
{
    //Delay to let shell load properly
    sceKernelDelayThread(4 * 1000 * 1000);

    SceUID CallbackUID = sceKernelCreateCallback("QMR_VblankCB", 0, VblankCallback, NULL);
    if (CallbackUID < 0)
        sceKernelExitThread(CallbackUID);

    SceInt32 ret = sceDisplayRegisterVblankStartCallback(CallbackUID);
    if (ret < 0)
        sceKernelExitThread(ret);

    while(!mainEnd) {
        sceKernelDelayThreadCB(0xFFFFFFFF);
    }

    ret = sceDisplayUnregisterVblankStartCallback(CallbackUID);
    if (ret < 0)
        sceKernelExitThread(ret);
    
    ret = sceKernelDeleteCallback(CallbackUID);
    if (ret < 0)
        sceKernelExitThread(ret);

    return sceKernelExitDeleteThread(0);
}

int load_thread(SceSize, void*)
{
    //Delay a bit, to let everything load
    sceKernelDelayThread(5 * 1000000);

    char dir[20] = {0};
    
    if(checkFileExist(PLUGINS_DIR))
        sce_paf_snprintf(dir, 20, PLUGINS_DIR);
    else if(checkFileExist(PLUGINS_DIR2))
        sce_paf_snprintf(dir, 20, PLUGINS_DIR2);
    else if(checkFileExist("ux0:"))
    {
        sceIoMkdir(PLUGINS_DIR, 0777);
        sce_paf_snprintf(dir, 20, PLUGINS_DIR);
    }
    else if(checkFileExist("ur0:"))
    {
        sceIoMkdir(PLUGINS_DIR2, 0777);
        sce_paf_snprintf(dir, 20, PLUGINS_DIR2);
    }

    SceUID id = -1;

    sceAppMgrGetIdByName(&id, "NPXS19999");

    SceIoDirent de;
    SceUID d = sceIoDopen(dir);

    int entries = 1;
    do 
    {
        entries = sceIoDread(d, &de);
        if(!SCE_STM_ISDIR(de.d_stat.st_mode))
        {
            if(sce_paf_strncmp(&de.d_name[sce_paf_strlen(de.d_name) - 5], "suprx", 5) == 0) // Check if it ends in "suprx"
            {
                char buff[0x400] = {0};
                sce_paf_snprintf(buff, 0x400, "%s/%s", dir, de.d_name);
                taiLoadStartModuleForPid(id, buff, 0, NULL, 0);
                print("Loading module at %s\n", buff);
            }
        }
    }while(entries > 0);
    sceIoDclose(d);
    return sceKernelExitDeleteThread(0);
}

#ifdef _DEBUG
int testWidgetThread(SceSize, void*)
{
    //Only filled during dev

    return sceKernelExitDeleteThread(0);
}
#endif

extern "C"
{
    int module_start(SceSize, void *)
    {
        sceClibPrintf("QuickMenuReborn, by Ibrahim\n");

        mainThreadID = sceKernelCreateThread("quickmenureborn", impose_thread, 248, SCE_KERNEL_128KiB, 0, 0, NULL);
        if(sceKernelStartThread(mainThreadID, 0, NULL) < 0)
        {
            print("Error loading main thread: 0x%X\n", mainThreadID);
            return SCE_KERNEL_START_NO_RESIDENT;
        }

        SceUID loadThreadId = sceKernelCreateThread("quickmenureborn_plugins", load_thread, 250, SCE_KERNEL_4KiB, 0, 0, NULL);
        if(sceKernelStartThread(loadThreadId, 0, NULL) < 0) 
        {
            print("Error loading load thread: 0x%X\n", loadThreadId);
            return SCE_KERNEL_START_NO_RESIDENT;
        }

#ifdef _DEBUG
        sceKernelStartThread(sceKernelCreateThread("test_widget_thread", testWidgetThread, 250, SCE_KERNEL_128KiB, 0, 0, NULL), 0, NULL);
#endif

        return SCE_KERNEL_START_SUCCESS;
    }

    int module_stop()
    {
        if(mainThreadID > 0)
        {
            mainEnd = true;
            sceKernelWaitThreadEnd(mainThreadID, NULL, NULL);
        }

        return SCE_KERNEL_STOP_SUCCESS;
    }
} 