# Usage

First thing I recommend doing is checking out the sample plugin

## Adding widgets

First thing you need for your widgets is a refrence id, it doesn't need to be anything specifically, just needs to be something not likely to be used elsewhere.  
Once you've gotten your id figured out you need to make a layout, this layout **will** need tweaking until it feels right so be prepared to experiment.  
To actually add the widget you can use the QuickMenuRebornRegisterWidget
# Separators
You may have noticed that the default widgets in the quick menu all have a line to separate themselves. You can use Separators in quickmenureborn to do this.

Once again these need refrence id's, You should only add separators at the start of your code so they appear at the top, DO NOT ADD SEPARATORS ON THE BOTTOM, this is for 2 reasons.
1. If your plugin is the first one, there is a separator between your widget and the accessability widgets
2. If your widgets are in the middle, they separate with the previous ones and blend in with the next ones

# Removing Widgets

Use the QuickMenuRebornRemoveWidget function and pass in the widget refrence id.  
To remove separators use the QuickMenuRebornRemoveSeparator function and pass in the refrence id.

For Example
```
    QuickMenuRebornRemoveWidget(BUTTON_REF_ID);
    QuickMenuRebornRemoveWidget(CHECKBOX_REF_ID);
    QuickMenuRebornRemoveWidget(TEXT_ID);
    QuickMenuRebornRemoveWidget(CHECKBOX_TEXT_ID);
    QuickMenuRebornRemoveWidget(PLANE_ID);
    QuickMenuRebornRemoveSeparator(SEPARATOR_ID); //Don't forget this!
```
  
I also recommend not removing widgets like checkboxes and buttons at times when the quickmenu is open, so do not remove widgets in the OnLoad and onPress/OnTrigger callbacks.  
This is to ensure the user doesn't trigger dead callbacks, which would crash shell.

# Kernel Plugins

QuickMenuReborn does not suppourt kernel plugins, it's not worth making a kernel plugin because kernel can't call userland functions  
I recommend devs to make a separete user plugin for their gui, it's not hard and use syscalls to send over the commands.
