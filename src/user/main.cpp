
#include "main.h"
#include "types.h"
#include "widgets.h"
#include "../quickmenureborn/qm_reborn.h"


SceUID mainThreadID = SCE_UID_INVALID_UID;

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

SceInt32 VblankCallback(SceUID notifyId, SceInt32 notifyCount, SceInt32 notifyArg, void* pCommon) 
{
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
#ifdef DEBUG
                    leakTestTask();
#endif
                    displayWidgets();
                    dispalyed = 1;
                }
            }
        }
        else dispalyed = 0;

    }
    return 0;
}

int impose_thread(SceSize, void *)
{
    //Delay to let shell load properly
    sceKernelDelayThread(4 * 1000 * 1000);
    
    if(semaID = sceKernelCreateSema(QM_REBORN_SEMA_NAME, SCE_KERNEL_ATTR_OPENABLE, SCE_KERNEL_1KiB, SCE_KERNEL_4KiB, NULL), semaID < 0) return sceKernelExitDeleteThread(0);
    else print("Make Sema success!\n");

    addInitialSpacer();

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

int checkDirExist(const char *path);

 SceUID taiLoadStartModuleForPid(SceUID pid, const char *path, int args, void *argp, int flags) {
  tai_module_args_t argg;
  argg.size = sizeof(argg);
  argg.pid = pid;
  argg.args = args;
  argg.argp = argp;
  argg.flags = flags;
  return taiLoadStartModuleForPidForUser(path, &argg);
}

int load_thread(SceSize, void*)
{
    //Delay a bit, to let everything load
    sceKernelDelayThread(5 * 1000000);

    char dir[20] = {0};
    
    if(checkDirExist(PLUGINS_DIR))
        sce_paf_snprintf(dir, 20, PLUGINS_DIR);
    else if(checkDirExist(PLUGINS_DIR2))
        sce_paf_snprintf(dir, 20, PLUGINS_DIR2);
    else if(checkDirExist("ux0:"))
    {
        sceIoMkdir(PLUGINS_DIR, 0777);
        sce_paf_snprintf(dir, 20, PLUGINS_DIR);
    }
    else if(checkDirExist("ur0:"))
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
            char buff[0x400] = {0};
            sce_paf_snprintf(buff, 0x400, "%s/%s", dir, de.d_name);
            taiLoadStartModuleForPid(id, buff, 0, NULL, 0);
            print("Loading module at %s\n", buff);
        }
    }while(entries > 0);
    sceIoDclose(d);
    return sceKernelExitDeleteThread(0);
}

extern "C"
{
    int module_start(SceSize, void *)
    {
        sceClibPrintf("QuickMenuReborn, by Ibrahim\n");
        mainThreadID = sceKernelCreateThread("quickmenureborn", impose_thread, 248, SCE_KERNEL_4KiB, 0, 0, NULL);
        if(sceKernelStartThread(mainThreadID, 0, NULL) < 0) return SCE_KERNEL_START_NO_RESIDENT;
        
        SceUID load_thread_id = sceKernelCreateThread("quickmenureborn_plugins", load_thread, 250, SCE_KERNEL_4KiB, 0, 0, NULL);
        if(load_thread_id < 0) return SCE_KERNEL_START_NO_RESIDENT;
        if(sceKernelStartThread(load_thread_id, 0, NULL) < 0) return SCE_KERNEL_START_NO_RESIDENT;
        return SCE_KERNEL_START_SUCCESS;
    }

    int module_stop()
    {
        if(mainThreadID > 0)
        {
            mainEnd = true;
            sceKernelWaitThreadEnd(mainThreadID, NULL, NULL);
        }
        if(semaID > 0)
            sceKernelDeleteSema(semaID);

        return SCE_KERNEL_STOP_SUCCESS;
    }
} 