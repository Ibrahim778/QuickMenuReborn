//QuickMenuReborn VDSuite Sample, By Ibrahim

//Tapper

#include <stdlib.h>
#include <kernel.h>
#include <libdbg.h>
#include <ctrl.h>
#include <quickmenureborn/qm_reborn.h>

//define a refrence id for each our widgets and textures, make sure they're all unique, like the ones below. Doesn't need to be anything specific, just something unique
#define BUTTON_REF_ID "qm_reborn_sample_button"
#define CHECKBOX_REF_ID "qm_reborn_sample_checkbox"
#define PLANE_ID "qm_reborn_sample_plane"
#define TEXT_ID "qm_reborn_sample_text"
#define CHECKBOX_TEXT_ID "qm_reborn_sample_checkbox_text"
#define SLIDEBAR_ID "qm_reborn_sample_slidebar"
#define SEPARATOR_ID "qm_reborn_sample_separator"
#define TEX_PLANE_ID "qm_reborn_sample_plane_for_tex"
#define TEXTURE_REF_ID "qm_reborn_sample_texture"

#define TEST_TEXTURE_PATH "ux0:app/VITASHELL/sce_sys/icon0.png"

//Set our current count
int count = 0;

//Declare our boolean
bool resetOnExit = false;

//Declare our function that will act as the callback for when our button is pressed, Format: BUTTON_HANDLER(name of function)
BUTTON_HANDLER(onPress)
{
    //Increment the count
    count++;

    //Make our buffer to pass to the function
    char newText[0x100]; // 0x400 is the max size for labels

    //Fill our buffer
    sceClibSnprintf(newText, 0x100, "You Pressed Me %d Times", count);
    float x = 400;
    
    if(count >= 100)
        x += 50;
    if(count >= 1000)
        x += 50;

    //Set new values
    QuickMenuRebornSetWidgetSize(BUTTON_REF_ID, x, 75, 0, 0);
    QuickMenuRebornSetWidgetLabel(BUTTON_REF_ID, newText);
    
}

ONLOAD_HANDLER(OnButtonLoad)
{    
    if(resetOnExit)
    {
        //Reset our count
        count = 0;

        //Update our widget with new size and text
        QuickMenuRebornSetWidgetSize(BUTTON_REF_ID, 200, 75, 0, 0);
        QuickMenuRebornSetWidgetLabel(BUTTON_REF_ID, "Press Me!");
    }
}

BUTTON_HANDLER(OnToggleCheckBox)
{
    resetOnExit = QuickMenuRebornGetCheckboxValue(CHECKBOX_REF_ID);
}

int module_start()
{
    QuickMenuRebornSeparator(SEPARATOR_ID, SCE_SEPARATOR_HEIGHT);

    //Get our checkboxes saved state
    int ret = QuickMenuRebornGetCheckboxValue(CHECKBOX_REF_ID);
    resetOnExit = ret == QMR_CONFIG_MGR_ERROR_NOT_EXIST ? false : ret;

    QuickMenuRebornRegisterWidget(TEXT_ID, NULL, text);
    QuickMenuRebornSetWidgetSize(TEXT_ID, SCE_PLANE_WIDTH, 50, 0, 0);
    QuickMenuRebornSetWidgetColor(TEXT_ID, 1,1,1,1);
    QuickMenuRebornSetWidgetPosition(TEXT_ID, 0, 0, 0, 0);
    QuickMenuRebornSetWidgetLabel(TEXT_ID, "Tapper");

    QuickMenuRebornRegisterWidget(PLANE_ID, NULL, plane);
    QuickMenuRebornSetWidgetSize(PLANE_ID, SCE_PLANE_WIDTH, 100, 0, 0);
    QuickMenuRebornSetWidgetColor(PLANE_ID, 1,1,1,0);

    QuickMenuRebornRegisterWidget(CHECKBOX_REF_ID, PLANE_ID, check_box);
    QuickMenuRebornSetWidgetSize(CHECKBOX_REF_ID, 48, 48, 0, 0);
    QuickMenuRebornSetWidgetColor(CHECKBOX_REF_ID, 1,1,1,1);
    QuickMenuRebornSetWidgetPosition(CHECKBOX_REF_ID, 350, 0, 0, 0);
    QuickMenuRebornAssignDefaultCheckBoxRecall(CHECKBOX_REF_ID);
    QuickMenuRebornAssignDefaultCheckBoxSave(CHECKBOX_REF_ID);
    QuickMenuRebornRegisterEventHanlder(CHECKBOX_REF_ID, QMR_BUTTON_RELEASE_ID, OnToggleCheckBox, NULL);

    QuickMenuRebornRegisterWidget(CHECKBOX_TEXT_ID, PLANE_ID, text);
    QuickMenuRebornSetWidgetColor(CHECKBOX_TEXT_ID, 1,1,1,1);
    QuickMenuRebornSetWidgetSize(CHECKBOX_TEXT_ID, 500, 75, 0, 0);
    QuickMenuRebornSetWidgetPosition(CHECKBOX_TEXT_ID, -255, 0, 0, 0);
    QuickMenuRebornSetWidgetLabel(CHECKBOX_TEXT_ID, "Reset On Exit");
    
    QuickMenuRebornRegisterWidget(BUTTON_REF_ID, NULL, button);
    QuickMenuRebornSetWidgetSize(BUTTON_REF_ID, 200, 75, 0, 0);
    QuickMenuRebornSetWidgetColor(BUTTON_REF_ID, 1,1,1,1);
    QuickMenuRebornRegisterEventHanlder(BUTTON_REF_ID, QMR_BUTTON_RELEASE_ID, onPress, NULL);
    QuickMenuRebornSetWidgetLabel(BUTTON_REF_ID, "Press Me!");
    QuickMenuRebornAssignOnLoadHandler(OnButtonLoad, BUTTON_REF_ID);

    SceIoStat s;
    if(sceIoGetstat(TEST_TEXTURE_PATH, &s) >= 0) //File Exists
    {
        QuickMenuRebornRegisterWidget(TEX_PLANE_ID, NULL, plane);
        QuickMenuRebornRegisterTexture(TEXTURE_REF_ID, TEST_TEXTURE_PATH);
        QuickMenuRebornSetWidgetSize(TEX_PLANE_ID, 128, 128, 0, 0);
        QuickMenuRebornSetWidgetColor(TEX_PLANE_ID, 1,1,1,1);
        QuickMenuRebornSetWidgetTexture(TEX_PLANE_ID, TEXTURE_REF_ID);
    }
    
    return SCE_KERNEL_START_SUCCESS;
}

int module_stop()
{
    //Remove our widgets from the list using our refrence ids, it will no longer be displayed
    QuickMenuRebornUnregisterWidget(BUTTON_REF_ID);
    QuickMenuRebornUnregisterWidget(CHECKBOX_REF_ID);
    QuickMenuRebornUnregisterWidget(TEXT_ID);
    QuickMenuRebornUnregisterWidget(CHECKBOX_TEXT_ID);
    QuickMenuRebornUnregisterWidget(PLANE_ID);
    QuickMenuRebornUnregisterTexture(TEST_TEXTURE_PATH);
    QuickMenuRebornRemoveSeparator(SEPARATOR_ID); //Don't forget this!
    return SCE_KERNEL_STOP_SUCCESS;
}
