
#include "main.h"
#include "utils.hpp"
#include "widgets.hpp"
#include "../quickmenureborn/qm_reborn.h"
#include "common.hpp"

bool displayed = false;

#ifdef _DEBUG
SceVoid leakTestTask(ScePVoid argp)
{
    if(imposePlugin == NULL) return;
    print("Free heap memory: %d\n", imposePlugin->memoryPool->GetFreeSize());
}
#endif

SceVoid ImposeCheck(ScePVoid args) 
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

                displayed = true;
                displayWidgets();
            
            }
        }
    }
    else displayed = false;
}

int impose_thread(SceSize, void *)
{
    //Delay to let shell load properly
    sceKernelDelayThread(4 * 1000 * 1000);

    common::Utils::AddMainThreadTask(ImposeCheck, NULL);
    
    #ifdef _DEBUG
    //common::Utils::AddMainThreadTask(leakTestTask, NULL);
    #endif


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

        SceUID mainThreadID = sceKernelCreateThread("quickmenureborn", impose_thread, 248, SCE_KERNEL_128KiB, 0, 0, NULL);
        if(sceKernelStartThread(mainThreadID, 0, NULL) < 0)
        {
            sceKernelDeleteThread(mainThreadID);
            print("Error loading main thread: 0x%X\n", mainThreadID);
            return SCE_KERNEL_START_NO_RESIDENT;
        }

        SceUID loadThreadId = sceKernelCreateThread("quickmenureborn_plugins", load_thread, 250, SCE_KERNEL_4KiB, 0, 0, NULL);
        if(sceKernelStartThread(loadThreadId, 0, NULL) < 0) 
        {
            sceKernelDeleteThread(loadThreadId);
            print("Error loading load thread: 0x%X\n", loadThreadId);
            return SCE_KERNEL_START_NO_RESIDENT;
        }

#ifdef _DEBUG
        SceUID testThreadID = sceKernelCreateThread("test_widget_thread", testWidgetThread, 250, SCE_KERNEL_128KiB, 0, 0, NULL);
        if(sceKernelStartThread(testThreadID, 0, NULL) < 0)
        {
            sceKernelDeleteThread(testThreadID);
            print("Error loading test widget thread: 0x%X\n", testWidgetThread);
        }
#endif

        return SCE_KERNEL_START_SUCCESS;
    }

    int module_stop()
    {
        common::Utils::RemoveMainThreadTask(ImposeCheck, NULL);

        if(widgetsDisplayed()) //Hide our current widgets, and free textures...
        {
            graphics::Texture transparentTexture; // Placeholder tex, to replace older widgets
            Resource::Element searchParam;
            searchParam.hash = Utils::GetHashById("_common_texture_check_mark");
            Plugin::LoadTexture(&transparentTexture, Plugin::Find("__system__common_resource"), &searchParam);

            Widget *w;
            node *n = currentWidgets.head;

            //Unassign textures    
            while(n != NULL)
            {
                w = (Widget *)n->widget.widget;

                w->PlayAnimationReverse(0, Widget::Animation_Reset);
                w->SetTexture(&transparentTexture, 0); //Replace textures so they are unused
                w->SetTextureBase(&transparentTexture);

                n = n->next;
            }

            //Delete Textures
            texNode *tn = currTextures.head;
            while(tn != NULL)
            {
                Utils::DeleteTexture(tn->texture, true);
                tn->texture = SCE_NULL;
                tn = tn->next;
            }
        }

        return SCE_KERNEL_STOP_SUCCESS;
    }
} 