#define QM_REBORN_INTERNAL
#include "../quickmenureborn/qm_reborn.h"
#include <kernel.h>
#include <libdbg.h>
#include "export_handler_internal.h"



static SceUID msgPipeUid = SCE_UID_INVALID_UID;

int module_start()
{
	msgPipeUid = sceKernelOpenMsgPipe(EXPORT_PIPE);
	if (msgPipeUid < 0) {
		SCE_DBG_LOG_INFO("sceKernelOpenMsgPipe() failed\n");
		return SCE_KERNEL_START_NO_RESIDENT;
	}
#ifdef DEBUG
    SCE_DBG_LOG_INFO("Exporter Loaded!\n");
#endif
    return SCE_KERNEL_START_SUCCESS;
}

int module_stop()
{
	if (msgPipeUid > 0)
		sceKernelCloseMsgPipe(msgPipeUid);

    return SCE_KERNEL_STOP_SUCCESS;
}

int updateWidget(widgetData *data, int flags)
{
    exportPacket packet;
	sceClibMemcpy(&packet.data, data, sizeof(widgetData));
    packet.type = update_widget;
    packet.updateFlags = flags;
    
    SceSize sent = 0;
    if(sceKernelSendMsgPipe(msgPipeUid, &packet, sizeof(packet), SCE_KERNEL_MSG_PIPE_MODE_WAIT | SCE_KERNEL_MSG_PIPE_MODE_FULL, &sent, NULL) != SCE_OK)
        return -2;
#ifdef DEBUG
    if(sent != sizeof(packet)) SCE_DBG_LOG_ERROR("Error couldn't send all the data!\n");
#endif
    return 0;
}

int addWidget(widgetData *data)
{
    exportPacket packet;
	sceClibMemcpy(&packet.data, data, sizeof(widgetData)); 
    packet.type = register_widget;
    
    SceSize sent = 0;
    if(sceKernelSendMsgPipe(msgPipeUid, &packet, sizeof(packet), SCE_KERNEL_MSG_PIPE_MODE_WAIT | SCE_KERNEL_MSG_PIPE_MODE_FULL, &sent, NULL) != SCE_OK)
        return -2;
#ifdef DEBUG
    if(sent != sizeof(packet)) SCE_DBG_LOG_ERROR("Error couldn't send all the data!\n");
#endif
    return 0;
}
//Doesn't work, I swear I tried it before and it did
int openQuickMenu()
{
    exportPacket packet;
    packet.type = open_quickmenu;

    SceSize sent = 0;

    if(sceKernelSendMsgPipe(msgPipeUid, &packet, sizeof(packet), SCE_KERNEL_MSG_PIPE_MODE_WAIT | SCE_KERNEL_MSG_PIPE_MODE_FULL, &sent, NULL) != SCE_OK)
        return -2;
    
#ifdef DEBUG
    if(sent != sizeof(packet)) SCE_DBG_LOG_ERROR("Error couldn't send all the data!");
#endif

    return 0;
}

int removeWidget(const char *refID)
{
    exportPacket packet;

    sceClibStrncpy((char *)&packet.data.refId, refID, sizeof(packet.data.refId));

    packet.type = unregister_widget;
    
    SceSize sent = 0;
    if(sceKernelSendMsgPipe(msgPipeUid, &packet, sizeof(packet), SCE_KERNEL_MSG_PIPE_MODE_WAIT | SCE_KERNEL_MSG_PIPE_MODE_FULL, &sent, NULL) != SCE_OK)
        return -2;
#ifdef DEBUG
    if(sent != sizeof(packet)) SCE_DBG_LOG_ERROR("Error couldn't send all the data!\n");
#endif
    return 0;
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

int QuickMenuRebornButton(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void (*OnLoad)(), void(*OnPress)(void))
{
    widgetData widget;
    sceClibMemset(&widget, 0, sizeof(widget));

    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    if(parentRefID != NULL) sceClibStrncpy(widget.parentRefId, parentRefID, sizeof(widget.parentRefId));

    widget.hasParent = parentRefID != NULL;

    widget.size = *Size;
    widget.pos = *Position;
    widget.type = button;
    widget.col = *Color;

    sceClibStrncpy(widget.data.ButtonData.label, Text, sizeof(widget.data.ButtonData.label));
    
    widget.data.ButtonData.onPress = OnPress;

    widget.isAdvanced = 0;

    widget.OnLoad = OnLoad;

    return addWidget(&widget);
}

int QuickMenuRebornCheckBox(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, void (*OnLoad)(), void(*OnToggle)(int state))
{
    widgetData widget;
    
    sceClibMemset(&widget, 0, sizeof(widget));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    if(parentRefID != NULL) sceClibStrncpy(widget.parentRefId, parentRefID, sizeof(widget.parentRefId));

    widget.hasParent = parentRefID != NULL;
    widget.size = *Size;
    widget.pos = *Position;
    widget.type = check_box;
    widget.col = *Color;

    widget.data.CheckBoxData.OnToggle = OnToggle;
    widget.OnLoad = OnLoad;
    
    widget.isAdvanced = 0;

    return addWidget(&widget);

}

int QuickMenuRebornText(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void (*OnLoad)())
{
    widgetData widget;
    
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    sceClibMemset(widget.parentRefId, 0, sizeof(widget.parentRefId));
    if(parentRefID != NULL) sceClibStrncpy(widget.parentRefId, parentRefID, sizeof(widget.parentRefId));

    widget.hasParent = parentRefID != NULL;

    widget.size = *Size;
    widget.pos = *Position;
    widget.col = *Color;
    widget.type = text;

    sceClibMemset(widget.data.TextData.label, 0, sizeof(widget.data.TextData.label));
    sceClibStrncpy(widget.data.TextData.label, Text, sizeof(widget.data.TextData.label));

    widget.OnLoad = OnLoad;

    widget.isAdvanced = 0;

    return addWidget(&widget);

}

int QuickMenuRebornPlane(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, void (*OnLoad)())
{
    widgetData widget;
    
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    sceClibMemset(widget.parentRefId, 0, sizeof(widget.parentRefId));
    if(parentRefID != NULL) sceClibStrncpy(widget.parentRefId, parentRefID, sizeof(widget.parentRefId));

    widget.hasParent = parentRefID != NULL;

    widget.size = *Size;
    widget.pos = *Position;
    widget.col = *Color;
    widget.type = plane;

    widget.OnLoad = OnLoad;

    widget.isAdvanced = 0;

    return addWidget(&widget);

}

int QuickMenuRebornSeparator(const char *refID)
{
    char sepID[256] = {0};
    sceClibSnprintf(sepID, sizeof(sepID), "qm_reborn_%s_separator", refID);

    vector4 size = makeWidgetVector4(825.0f,2.0f,0.0f,0.0f), pos = makeWidgetVector4(0,0,0,0);
    widgetColor col = makeWidgetColor(.75f,.75f,.75f,.75f);
    QuickMenuRebornPlane(sepID, NULL, &size, &pos, &col, NULL);

    return 0;
}

int QuickMenuRebornUpdateButton(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void(*OnPress)(void), void (*OnLoad)(), int flags)
{
    sceKernelDelayThread(1000);
    print("Updating button with refID: %s\n", refID);
    widgetData widget;
    sceClibMemset(&widget, 0, sizeof(widget));

    print("Made and set widget\n");

    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

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
        sceClibStrncpy(widget.data.ButtonData.label, Text, sizeof(widget.data.ButtonData.label));
    print("Copied Text\n");
    if(flags & UPDATE_EVENT)
        widget.data.ButtonData.onPress = OnPress;
    print("Set Event\n");
    if(flags & UPDATE_LOAD)
        widget.OnLoad = OnLoad;
    print("Set LoadEvent\n");

    return updateWidget(&widget, flags);
}

int QuickMenuRebornUpdateCheckBox(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, void(*OnToggle)(int state), void (*OnLoad)(), int flags)
{
    widgetData widget;
    
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));


    if(Size != NULL) widget.size = *Size;
    if(Position != NULL) widget.pos = *Position;
    widget.type = check_box;
    if(Color != NULL) widget.col = *Color;

    widget.data.CheckBoxData.OnToggle = OnToggle;
    widget.OnLoad = OnLoad;

    widget.isAdvanced = 0;


    return updateWidget(&widget, flags);
}

int QuickMenuRebornUpdateText(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Text, void (*OnLoad)(), int flags)
{
    widgetData widget;
    
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    if(Size != NULL) widget.size = *Size;
    if(Position != NULL) widget.pos = *Position;
    if(Color != NULL) widget.col = *Color;
    widget.type = text;

    if(Text != NULL)
    {
        sceClibMemset(widget.data.ButtonData.label, 0, sizeof(widget.data.ButtonData.label));
        sceClibStrncpy(widget.data.ButtonData.label, Text, sizeof(widget.data.ButtonData.label));        
    }

    widget.OnLoad = OnLoad;

    widget.isAdvanced = 0;

    return updateWidget(&widget, flags);

}

int QuickMenuRebornUpdatePlane(const char *refID, vector4 *Size, vector4 *Position, widgetColor *Color, void (*OnLoad)(), int flags)
{
    widgetData widget;
    
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    if(Size != NULL) widget.size = *Size;
    if(Position != NULL) widget.pos = *Position;
    if(Color != NULL) widget.col = *Color;
    widget.type = plane;

    widget.OnLoad = OnLoad;

    widget.isAdvanced = 0;

    return updateWidget(&widget, flags);

}

int QuickMenuRebornAddAdvancedWidget(const char *refID, const char *parentRefID, vector4 *Size, vector4 *Position, widgetColor *Color, const char *Type, const char *idType, void (*OnLoad)())
{
    widgetData widget;

    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));
    
    sceClibMemset(widget.parentRefId, 0, sizeof(widget.parentRefId));
    if(parentRefID != NULL) sceClibStrncpy(widget.parentRefId, parentRefID, sizeof(widget.parentRefId));

    if(Size != NULL) widget.size = *Size;
    if(Position != NULL) widget.pos = *Position;
    if(Color != NULL) widget.col = *Color;

    widget.isAdvanced = true;

    widget.hasParent = parentRefID != NULL;

    sceClibMemset(&widget.adata, 0, sizeof(widget.adata));

    sceClibStrncpy(widget.adata.idType, idType, sizeof(widget.adata.idType));
    sceClibStrncpy(widget.adata.type, Type, sizeof(widget.adata.type));

    widget.OnLoad = OnLoad;

    return addWidget(&widget);
}