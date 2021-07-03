#include "main.h"
#include "types.h"
#include "widgets.h"
#include "quickmenureborn/qm_reborn.h"
#include "event_handler.cpp"

SceUID mainThreadID = -1;

int setupWidgets()
{
    qm_reborn_ezseparator2("QuickMenuReborn");
    qm_reborn_eztext("qm_internal_hombrew_text", NULL, makeWidgetVector4(165, 75, 0, 0), makeWidgetVector4(-300,0,0,0), makeWidgetColor(1,1,1,1), "Homebrew", 0);
    
    /*
    qm_reborn_ezseparator("x1vita");
    qm_reborn_eztext("x1vita_text", NULL, makeWidgetVector4(165,75,0,0), makeWidgetVector4(0,0, 0, 0), makeWidgetColor(1,1,1,1), "X1Vita", 0);
    qm_reborn_ezplane("x1vita_options", NULL, makeWidgetVector4(825, 100, 0, 0), makeWidgetVector4(0,0,0,0), makeWidgetColor(1,1,1,0));

    qm_reborn_eztext("x1vita_toggle_text", "x1vita_options", makeWidgetVector4(625, 75, 0, 0), makeWidgetVector4(-165,0,0,0), makeWidgetColor(1,1,1,1), "Swap Triggers and Bumpers", 0);
    qm_reborn_ezcheckbox("x1vita_toggle", "x1vita_options", makeWidgetVector4(10,10,0,0), makeWidgetVector4(345,0,0,0), makeWidgetColor(0,1,0,1), NULL);
    */
    return 0;
}

int impose_thread(SceSize args, void *argp)
{
    int ret;
    ret = initWidgets();
    FAILTHREAD_IF(ret < 0);
    setupWidgets();
    ret = displayWidgets();
    FAILTHREAD_IF(ret < 0);
    return sceKernelExitDeleteThread(0);
}

extern "C"
{
    int _start(SceSize args, void *argp) __attribute__((weak, alias("module_start")));
    int module_start(SceSize args, void *argp)
    {
        QMEventHandler eh;
        mainThreadID = sceKernelCreateThread("quickmenureborn", impose_thread, 0x10000100, 0x10000, 0, 0, NULL);
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