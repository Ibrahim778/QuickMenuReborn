# Usage

First thing I recommend doing is checking out the sample plugin

## Adding widgets

First thing you need for your widgets is a refrence id, it doesn't need to be anything specifically, just needs to be something not likely to be used elsewhere.  
Once you've gotten your id figured out you need to make a layout, this layout **will** need tweaking until it feels right so be prepared to experiment.  
To actually add the widget you can use the QuickMenuRebornRegisterWidget, after that you will need to adjust some properties for it to show up. You can use QuickMenuRebornSetWidgetColor and QuickMenuRebornSetWidgetSize to adjust the size, then you can use QuickMenuRebornSetWidgetPosition to adjust the position.

### Assigning and managing callbacks

Checkboxes, slidebars and buttons can have callbacks assigned to them, use the QuickMenuRebornRegisterEventHanlder. In order to save checkbox / slidebar values / positions and reassign them when the quickmenu opens you must assign recall and save callbacks. You can either use your own custom ones and use QuickMenuRebornAssignRecallHandler and QuickMenuRebornAssignSaveHandler or you can use premade ones with QuickMenuRebornAssignDefaultSliderSave and QuickMenuRebornAssignDefaultSliderRecall and the same for other widgets. Other than that you can assign OnLoad callbacks with QuickMenuRebornAssignOnLoadHandler.

### Textures

You will first need to register a texture before you can use it, Use QuickMenuRebornRegisterTexture("texture_refrence_id", "path/to/file")
You can then use the refrence id you passed to assign the texture to the widget, use QuickMenuRebornSetWidgetTexture("widget_refrence_id", "texture_refrence_id")
For Example:
```C
    SceIoStat s;
    if(sceIoGetstat(TEST_TEXTURE_PATH, &s) >= 0) //File Exists
    {
        QuickMenuRebornRegisterWidget(TEX_PLANE_ID, NULL, plane);
        QuickMenuRebornRegisterTexture(TEXTURE_REF_ID, TEST_TEXTURE_PATH);
        QuickMenuRebornSetWidgetSize(TEX_PLANE_ID, 128, 128, 0, 0);
        QuickMenuRebornSetWidgetColor(TEX_PLANE_ID, 1,1,1,1);
        QuickMenuRebornSetWidgetTexture(TEX_PLANE_ID, TEXTURE_REF_ID);
    }
```

## Separators
You may have noticed that the default widgets in the quick menu all have a line to separate themselves. You can use Separators in quickmenureborn to do this.

Once again these need refrence id's, You should only add separators at the start of your code so they appear at the top, DO NOT ADD SEPARATORS ON THE BOTTOM, this is for 2 reasons.
1. If your plugin is the first one, there is a separator between your widget and the accessability widgets
2. If your widgets are in the middle, they separate with the previous ones and blend in with the next ones

## Removing Widgets

Use the QuickMenuRebornRemoveWidget function and pass in the widget refrence id.  
To remove separators use the QuickMenuRebornRemoveSeparator function and pass in the refrence id.

For Example
```c
    QuickMenuRebornRemoveWidget(BUTTON_REF_ID);
    QuickMenuRebornRemoveWidget(CHECKBOX_REF_ID);
    QuickMenuRebornRemoveWidget(TEXT_ID);
    QuickMenuRebornRemoveWidget(CHECKBOX_TEXT_ID);
    QuickMenuRebornRemoveWidget(PLANE_ID);
    QuickMenuRebornRemoveSeparator(SEPARATOR_ID); //Don't forget this!
```
  

# Kernel Plugins

QuickMenuReborn does not suppourt kernel plugins, it's not worth making a kernel plugin because kernel can't call userland functions  
I recommend devs to make a separete user plugin for their gui, it's not hard and use syscalls to send over the commands.
I may create a kernel / user registry bridge later though.