# Usage

First thing I recommend doing is checking out the sample plugin

## Adding widgets

First thing you need for your widgets is a refrence id, it doesn't need to be anything specifically, just needs to be something not likely to be used elsewhere.  
Once you've gotten your id figured out you need to make a layout, this layout **will** need tweaking until it feels right so be prepared to experiment.  
To actually add the widget there are 2 options, use the function QuickMenuRebornAddWidget after creating a widgetData struct and populating the fields.  
Or Use the functions made for individual widgets which will populate the fields for you QuickMenuRebornButton for buttoms QuickMenuRebornCheckBoxes for checkboxes etc.

For example. Adding a button: 
```
    size = makeWidgetVector4Int(200,75,0,0);
    pos = makeCommonWidgetVector4Int(0);
    col = makeWidgetColorInt(1, 1, 1, 1) //range is 0-1, you can alternativly use defines (already in the headers) like COLOR_WHITE
    
    QuickMenuRebornButton(BUTTON_REF_ID, NULL, &size, &pos, &col, "Press Me!", OnButtonLoad, onPress); //Last 2 parameters are event handlers, you can pass null
```
  
    
## CheckBox save system

Every added checkbox will have it's value saved on each press, this save is identified by the refrence Id, so if you change it, it will act as if you reset it.
The last parameter for the checkbox function takes in one of 3 values
```
CHECKBOX_ON
CHECKBOX_OFF
CHECKBOX_PREV_STATE
```
  
These will determine the initial value of the checkbox every time the quick menu is opened. The last one will use the previous saved state.  
For Example:
```
    //Checkbox
    col = COLOR_WHITE;
    size = makeWidgetVector4Int(50, 50, 0, 0);
    pos = makeWidgetVector4Int(350, 0, 0, 0);

    QuickMenuRebornCheckBox(CHECKBOX_REF_ID, NULL, &size, &pos, &col, NULL, OnToggleCheckBox, CHECKBOX_PREV_STATE);
```

If you want to get the previous saved state of the checkbox via code you can use QuickMenuRebornGetCheckBoxState function and pass the refrence Id of the checkbox.  
For Example:
```
    //Get our checkboxes saved state
    int ret = QuickMenuRebornGetCheckBoxState(CHECKBOX_REF_ID);
    // Check if it was successful, if not default to 0 (false)
    if(ret == CONFIG_MGR_ERROR_NOT_EXIST) resetOnExit = 0; 
    else resetOnExit = ret;
```

# Separators
You may have noticed that the default widgets in the quick menu all have a line to separate themselves. You can use Separators in quickmenureborn to imitate this (it's not perfect, but close)

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
