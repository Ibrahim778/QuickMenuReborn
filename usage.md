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
    
    QuickMenuRebornButton(BUTTON_REF_ID, NULL, &size, &pos, &col, "Press Me!", OnButtonLoad, onPress);
```
  
## CheckBox save system

