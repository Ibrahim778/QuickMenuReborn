#include "main.h"
#include "quickmenureborn/qm_reborn.h"

__attribute__((used))
int qm_reborn_eztext(char *refID, char *parentRefID, vector4 Size, vector4 Position, widgetColor Color, char *Text, int isBold)
{
    widgetData data;
    data.col = Color;
    data.pos = Position;
    data.size = Size;
    data.type = text;
    data.refId = refID;
    data.parentRefId = parentRefID;
    data.data.TextData.label = Text;
    data.data.TextData.isbold = isBold;

    char *id = registerWidget(data);
    FAIL_IF(sceClibStrcmp(id, data.refId));
    return 0;
}

__attribute__((used))
int qm_reborn_ezbutton(char *refID, char *parentRefID, vector4 Size, vector4 Position, widgetColor Color, char *Text, void (*OnPress)(void))
{
    widgetData data;
    data.col = Color;
    data.pos = Position;
    data.parentRefId = parentRefID;
    data.size = Size;
    data.data.ButtonData.onPress = OnPress;
    data.type = button;
    data.refId = refID;
    data.data.ButtonData.label = Text;

    char *id = registerWidget(data);
    FAIL_IF(sceClibStrcmp(id, data.refId));
    return 0;
}

__attribute__((used))
int qm_reborn_ezcheckbox(char *refID, char *parentRefID, vector4 Size, vector4 Position, widgetColor Color, void (*onToggle)(int state))
{
    widgetData data;
    data.col = Color;
    data.pos = Position;
    data.parentRefId = parentRefID;
    data.size = Size;
    data.type = check_box;
    data.refId = refID;
    data.data.CheckBoxData.OnToggle = onToggle;

    char *id = registerWidget(data);
    FAIL_IF(sceClibStrcmp(id, data.refId));
    return 0;
}

__attribute__((used))
int qm_reborn_ezplane(char *refID, char *parentRefID, vector4 Size, vector4 Position, widgetColor Color)
{
    widgetData data;
    data.refId = refID;
    data.parentRefId = parentRefID;
    data.pos = Position;
    data.size = Size;
    data.col = Color;
    data.type = plane;

    FAIL_IF(sceClibStrcmp(registerWidget(data), data.refId));
    return 0;
}

__attribute__((used))
int qm_reborn_ezseparator2(char *refID)
{
    sceClibPrintf("MAking spac\n");
    //Spacer
    char spacerId[0x100];
    sceClibSnprintf(spacerId, 0x100, "qm_reborn_%s_spacer", refID);
    qm_reborn_ezplane(spacerId, NULL, makeWidgetVector4(825,25,0,0), makeWidgetVector4(0,0,0,0), makeWidgetColor(1,1,1,0));
    sceClibPrintf("Made spac\n");

    sceClibPrintf("Making L\n");
    //Line
    char lineId[0x100];
    sceClibSnprintf(lineId, 0x100, "qm_reborn_%s_line", refID);
    qm_reborn_ezplane(lineId, NULL, makeWidgetVector4(825,2,0,0), makeWidgetVector4(0,0,0,0), makeWidgetColor(.75f,.75f,.75f,.75f));
    sceClibPrintf("Made L\n");
    return 0;
}

__attribute__((used))
int qm_reborn_ezseparator(char *refID)
{

    //Line
    char lineId[0x100];
    sceClibSnprintf(lineId, 0x100, "qm_reborn_%s_line", refID);
    qm_reborn_ezplane(lineId, NULL, makeWidgetVector4(825,2,0,0), makeWidgetVector4(0,0,0,0), makeWidgetColor(.75f,.75f,.75f,.75f));
    return 0;
}