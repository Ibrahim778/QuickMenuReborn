#define QM_REBORN_INTERNAL
#include "../quickmenureborn/qm_reborn.h"
#include <kernel.h>
#include <libdbg.h>
#include "main.h"
#include "widgets.h"
#include "config_mgr.h"


int QuickMenuRebornUpdateWidget(widgetData *data, int flags)
{
    return update_Widget(data, flags);
}

int QuickMenuRebornAddWidget(widgetData *data)
{
    return registerWidget(data);
}

int QuickMenuRebornRemoveWidget(const char *refID)
{
    return unregisterWidget(refID);
}

vector4 makeWidgetVector4(float x, float y, float z, float w)
{
    vector4 v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
    return v;
}

widgetColor makeWidgetColor(float r, float g, float b, float a)
{
    widgetColor c;
    c.r = r;
    c.b = b;
    c.g = g;
    c.a = a;
    return c;
}

int QuickMenuRebornRemoveSeparator(const char *refID)
{
    char id[0x100];
    sce_paf_snprintf(id, sizeof(id), "qm_reborn_%s_separator", refID);

    return unregisterWidget(id);
}

int QuickMenuRebornButton(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void (*OnLoad)(), void(*OnPress)(void))
{
    widgetData widget;
    sceClibMemset(&widget, 0, sizeof(widget));

    sce_paf_strncpy(widget.refId, refID, sizeof(widget.refId));

    if(parentRefID != NULL) sce_paf_strncpy(widget.parentRefId, parentRefID, sizeof(widget.parentRefId));

    widget.hasParent = parentRefID != NULL;

    widget.size = *Size;
    widget.pos = *Position;
    widget.type = button;
    widget.col = *Color;

    sce_paf_strncpy(widget.data.ButtonData.label, Text, sizeof(widget.data.ButtonData.label));
    
    widget.data.ButtonData.onPress = OnPress;

    widget.isAdvanced = 0;

    widget.OnLoad = OnLoad;

    return registerWidget(&widget);
}

int QuickMenuRebornCheckBox(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, void (*OnLoad)(), void(*OnToggle)(int state), CheckBoxState state)
{
    widgetData widget;
    
    sceClibMemset(&widget, 0, sizeof(widget));
    sce_paf_strncpy(widget.refId, refID, sizeof(widget.refId));

    if(parentRefID != NULL) sce_paf_strncpy(widget.parentRefId, parentRefID, sizeof(widget.parentRefId));

    widget.hasParent = parentRefID != NULL;
    widget.size = *Size;
    widget.pos = *Position;
    widget.type = check_box;
    widget.col = *Color;

    widget.data.CheckBoxData.OnToggle = OnToggle;
    widget.OnLoad = OnLoad;
    
    widget.data.CheckBoxData.state = state;

    widget.isAdvanced = 0;

    return registerWidget(&widget);

}

int QuickMenuRebornText(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void (*OnLoad)())
{
    widgetData widget;
    
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sce_paf_strncpy(widget.refId, refID, sizeof(widget.refId));

    sceClibMemset(widget.parentRefId, 0, sizeof(widget.parentRefId));
    if(parentRefID != NULL) sce_paf_strncpy(widget.parentRefId, parentRefID, sizeof(widget.parentRefId));

    widget.hasParent = parentRefID != NULL;

    widget.size = *Size;
    widget.pos = *Position;
    widget.col = *Color;
    widget.type = text;

    sceClibMemset(widget.data.TextData.label, 0, sizeof(widget.data.TextData.label));
    sce_paf_strncpy(widget.data.TextData.label, Text, sizeof(widget.data.TextData.label));

    widget.OnLoad = OnLoad;

    widget.isAdvanced = 0;

    return registerWidget(&widget);

}

int QuickMenuRebornPlane(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, void (*OnLoad)())
{
    widgetData widget;
    
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sce_paf_strncpy(widget.refId, refID, sizeof(widget.refId));

    sceClibMemset(widget.parentRefId, 0, sizeof(widget.parentRefId));
    if(parentRefID != NULL) sce_paf_strncpy(widget.parentRefId, parentRefID, sizeof(widget.parentRefId));

    widget.hasParent = parentRefID != NULL;

    widget.size = *Size;
    widget.pos = *Position;
    widget.col = *Color;
    widget.type = plane;

    widget.OnLoad = OnLoad;

    widget.isAdvanced = 0;

    return registerWidget(&widget);

}

int QuickMenuRebornSeparator(const char *refID)
{
    char sepID[256] = {0};
    sce_paf_snprintf(sepID, sizeof(sepID), "qm_reborn_%s_separator", refID);

    vector4 size = makeWidgetVector4(825.0f,2.0f,0.0f,0.0f), pos = makeWidgetVector4(0,0,0,0);
    widgetColor col = makeWidgetColor(.75f,.75f,.75f,.75f);
    QuickMenuRebornPlane(sepID, NULL, &size, &pos, &col, NULL);

    return 0;
}

int QuickMenuRebornUpdateButton(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void(*OnPress)(void), void (*OnLoad)(), int flags)
{
    print("Updating button with refID: %s\n", refID);
    widgetData widget;
    sceClibMemset(&widget, 0, sizeof(widget));

    print("Made and set widget\n");

    sce_paf_strncpy(widget.refId, refID, sizeof(widget.refId));

    print("Copied refID\n");

    if(Size != NULL) widget.size = *Size;
    print("Set size\n");
    if(Position != NULL) widget.pos = *Position;
    print("Set Pos\n");
    widget.type = button;
    print("Set Type\n");
    if(Color != NULL) widget.col = *Color;
    print("Set Col\n");
    if(Text != NULL)
        sce_paf_strncpy(widget.data.ButtonData.label, Text, sizeof(widget.data.ButtonData.label));
    print("Copied Text\n");
    if(flags & UPDATE_EVENT)
        widget.data.ButtonData.onPress = OnPress;
    print("Set Event\n");
    if(flags & UPDATE_LOAD)
        widget.OnLoad = OnLoad;
    print("Set LoadEvent\n");

    return QuickMenuRebornUpdateWidget(&widget, flags);
}

int QuickMenuRebornUpdateCheckBox(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, void(*OnToggle)(int state), void (*OnLoad)(), int flags)
{
    widgetData widget;
    
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sce_paf_strncpy(widget.refId, refID, sizeof(widget.refId));


    if(Size != NULL) widget.size = *Size;
    if(Position != NULL) widget.pos = *Position;
    widget.type = check_box;
    if(Color != NULL) widget.col = *Color;

    widget.data.CheckBoxData.OnToggle = OnToggle;
    widget.OnLoad = OnLoad;

    widget.isAdvanced = 0;


return QuickMenuRebornUpdateWidget(&widget, flags);
}

int QuickMenuRebornUpdateText(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void (*OnLoad)(), int flags)
{
    widgetData widget;
    
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sce_paf_strncpy(widget.refId, refID, sizeof(widget.refId));

    if(Size != NULL) widget.size = *Size;
    if(Position != NULL) widget.pos = *Position;
    if(Color != NULL) widget.col = *Color;
    widget.type = text;

    if(Text != NULL)
    {
        sceClibMemset(widget.data.ButtonData.label, 0, sizeof(widget.data.ButtonData.label));
        sce_paf_strncpy(widget.data.ButtonData.label, Text, sizeof(widget.data.ButtonData.label));        
    }

    widget.OnLoad = OnLoad;

    widget.isAdvanced = 0;

return QuickMenuRebornUpdateWidget(&widget, flags);

}

int QuickMenuRebornUpdatePlane(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, void (*OnLoad)(), int flags)
{
    widgetData widget;
    
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sce_paf_strncpy(widget.refId, refID, sizeof(widget.refId));

    if(Size != NULL) widget.size = *Size;
    if(Position != NULL) widget.pos = *Position;
    if(Color != NULL) widget.col = *Color;
    widget.type = plane;

    widget.OnLoad = OnLoad;

    widget.isAdvanced = 0;

return QuickMenuRebornUpdateWidget(&widget, flags);

}

int QuickMenuRebornAddAdvancedWidget(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Type, const char *idType, void (*OnLoad)())
{
    widgetData widget;

    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sce_paf_strncpy(widget.refId, refID, sizeof(widget.refId));
    
    sceClibMemset(widget.parentRefId, 0, sizeof(widget.parentRefId));
    if(parentRefID != NULL) sce_paf_strncpy(widget.parentRefId, parentRefID, sizeof(widget.parentRefId));

    if(Size != NULL) widget.size = *Size;
    if(Position != NULL) widget.pos = *Position;
    if(Color != NULL) widget.col = *Color;

    widget.isAdvanced = true;

    widget.hasParent = parentRefID != NULL;

    sceClibMemset(&widget.adata, 0, sizeof(widget.adata));

    sce_paf_strncpy(widget.adata.idType, idType, sizeof(widget.adata.idType));
    sce_paf_strncpy(widget.adata.type, Type, sizeof(widget.adata.type));

    widget.OnLoad = OnLoad;

    return registerWidget(&widget);
}

int QuickMenuRebornGetCheckBoxState(const char *refID)
{
    return readCheckBoxState(refID);
}