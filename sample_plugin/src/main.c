//QuickMenuReborn VDSuite Sample, By Ibrahim

//Tapper

#include <stdlib.h>
#include <kernel.h>
#include <kernel/libkernel.h>
#include <libdbg.h>
#include <ctrl.h>
#include <quickmenureborn/qm_reborn.h>

//define a refrence id for each our widgets, make sure they're all unique, like the ones below. Doesn't need to be anything specific, just something unique
#define BUTTON_REF_ID "qm_reborn_sample_button"
#define CHECKBOX_REF_ID "qm_reborn_sample_checkbox"
#define PLANE_ID "qm_reborn_sample_plane"
#define TEXT_ID "qm_reborn_sample_text"
#define CHECKBOX_TEXT_ID "qm_reborn_sample_checkbox_text"
#define SEPARATOR_ID "qm_reborn_sample_separator"

//Set our current count
int count = 0;

//Declare our boolean
bool resetOnExit = false;

//Declare our function that will act as the callback for when our burron is pressed, Format: BUTTON_HANDLER(name of function)
BUTTON_HANDLER(onPress)
{
    //Increment the count
    count++;

    //Make our buffer to pass to the function
    char newText[0x100]; // 0x100 is the max size for everything

    //Fill our buffer
    sceClibSnprintf(newText, 0x100, "You Pressed Me %d Times", count);

    //Declare our size variable to pass to the update function
    vector4 size = makeWidgetVector4Int(400,75,0,0);
    
    //Adjust the size according to the number of characters
    if(count >= 100)
        size.x += 50;
    if(count >= 1000)
        size.x += 50;

    //Call our update function with the parameters we want to update along with the corresponding flags
    QuickMenuRebornUpdateButton(BUTTON_REF_ID, &size, NULL, NULL, newText, NULL, NULL, UPDATE_TEXT | UPDATE_SIZE);
}

ONLOAD_HANDLER(OnButtonLoad)
{    
    if(resetOnExit)
    {
        //Reset our count
        count = 0;
        //Update our widget with new size and text
        vector4 size = makeWidgetVector4(200, 75, 0, 0);
        QuickMenuRebornUpdateButton(BUTTON_REF_ID, &size, NULL, NULL, "Press Me!", NULL, NULL, UPDATE_TEXT | UPDATE_SIZE);
    }
}

CHECKBOX_HANDLER(OnToggleCheckBox)
{
    resetOnExit = checked;
}

int module_start()
{
    //Get our checkboxes saved state
    int ret = QuickMenuRebornGetCheckBoxState(CHECKBOX_REF_ID);
    if(ret == CONFIG_MGR_ERROR_NOT_EXIST) resetOnExit = 0;
    else resetOnExit = ret;
    
    
    //Make a separator for our widgets, this is just for looks (adds some spacing and a white line at the top, this ensures the accessability menu isn't ruined and other plugins get thier share of space)
    QuickMenuRebornSeparator(SEPARATOR_ID);
    //First Widget we make, the text

    //Create and set the size and position variables
    vector4 size = makeWidgetVector4Int(200, 75, 0, 0), pos = makeCommonWidgetVector4Int(0);

    //Same for color variable
    widgetColor col = COLOR_WHITE;

    //Call Export to register function with all the basic parameters, along with the refrence id we made earlier and the onPress callback function, once a widget has been registered with an id you cannot change it without unregistering (removing) it
    QuickMenuRebornText(TEXT_ID, NULL, &size, &pos, &col, "Tapper", NULL);

    //Second widget we make, a plane (box) for our Text and Check Box, which will be child objects

    size = makeWidgetVector4(SCE_PLANE_WIDTH, 100.0f, 0.0f, 0.0f);
    col = COLOR_TRANSPARENT;
    QuickMenuRebornPlane(PLANE_ID, NULL, &size, &pos, &col, NULL);

    //Checkbox
    col = COLOR_WHITE;
    size = makeWidgetVector4Int(50, 50, 0, 0);
    pos = makeWidgetVector4Int(350, 0, 0, 0);

    //Make our checkbox as a child of the plane, so we can put the text besides it
    QuickMenuRebornCheckBox(CHECKBOX_REF_ID, PLANE_ID, &size, &pos, &col, NULL, OnToggleCheckBox, CHECKBOX_PREV_STATE);

    //Fourth widget the checkbox text
    size = makeWidgetVector4Int(500, 75, 0, 0);
    pos = makeWidgetVector4Int(-225, 0, 0, 0);

    QuickMenuRebornText(CHECKBOX_TEXT_ID, PLANE_ID, &size, &pos, &col, "Reset on exit", NULL);
    
    //Last Widget We make a button

    size = makeWidgetVector4Int(200,75,0,0);
    pos = makeCommonWidgetVector4Int(0);
    //We use the same color as before

    QuickMenuRebornButton(BUTTON_REF_ID, NULL, &size, &pos, &col, "Press Me!", OnButtonLoad, onPress);


    return SCE_KERNEL_START_SUCCESS;
}

int module_stop()
{
    //Remove our widgets from the list using our refrence ids, it will no longer be displayed
    QuickMenuRebornRemoveWidget(BUTTON_REF_ID);
    QuickMenuRebornRemoveWidget(CHECKBOX_REF_ID);
    QuickMenuRebornRemoveWidget(TEXT_ID);
    QuickMenuRebornRemoveWidget(CHECKBOX_TEXT_ID);
    QuickMenuRebornRemoveWidget(PLANE_ID);
    QuickMenuRebornRemoveSeparator(SEPARATOR_ID); //Don't forget this!
    return SCE_KERNEL_STOP_SUCCESS;
}