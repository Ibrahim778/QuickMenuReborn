#define QM_REBORN_INTERNAL
#include "../quickmenureborn/qm_reborn.h"
#include <kernel.h>
#include <libdbg.h>

static SceUID msgPipeUid = SCE_UID_INVALID_UID;

int module_start()
{
	msgPipeUid = sceKernelOpenMsgPipe(EXPORT_PIPE);
	if (msgPipeUid < 0) {
		SCE_DBG_LOG_INFO("sceKernelOpenMsgPipe() failed\n");
		return SCE_KERNEL_START_NO_RESIDENT;
	}

    SCE_DBG_LOG_INFO("Exporter Loaded!\n");
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
    if(sceKernelSendMsgPipe(msgPipeUid, &packet, sizeof(packet), SCE_KERNEL_MSG_PIPE_MODE_DONT_WAIT | SCE_KERNEL_MSG_PIPE_MODE_FULL, &sent, NULL) != SCE_OK)
        return -2;
#ifdef DEBUG
    if(sent != sizeof(packet)) SCE_DBG_LOG_ERROR("Error couldn't send all the data!\n");
#endif
    return 0;
}

int addWidget(widgetData *data)
{
    SCE_DBG_LOG_INFO("Got data %s\n", data->refId);
    exportPacket packet;
	sceClibMemcpy(&packet.data, data, sizeof(widgetData)); 
    packet.type = register_widget;
    
    SceSize sent = 0;
    if(sceKernelSendMsgPipe(msgPipeUid, &packet, sizeof(packet), SCE_KERNEL_MSG_PIPE_MODE_DONT_WAIT | SCE_KERNEL_MSG_PIPE_MODE_FULL, &sent, NULL) != SCE_OK)
        return -2;
#ifdef DEBUG
    if(sent != sizeof(packet)) SCE_DBG_LOG_ERROR("Error couldn't send all the data!\n");
#endif
    return 0;
}

void removeWidget(const char *refID)
{
    exportPacket packet;

    sceClibMemset(&packet, 0, sizeof(packet));
    sceClibStrncpy((char *)&packet.data.refId, refID, sizeof(packet.data.refId));

    packet.type = unregister_widget;
    
    SceSize sent = 0;
    if(sceKernelSendMsgPipe(msgPipeUid, &packet, sizeof(packet), SCE_KERNEL_MSG_PIPE_MODE_DONT_WAIT | SCE_KERNEL_MSG_PIPE_MODE_FULL, &sent, NULL) != SCE_OK)
        return;
#ifdef DEBUG
    if(sent != sizeof(packet)) SCE_DBG_LOG_ERROR("Error couldn't send all the data!\n");
#endif
    return;
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

int QuickMenuRebornButton(const char *refID, const char *parentRefID, vector4 Size, vector4 Position, widgetColor Color, const char *Text, void(*OnPress)(void))
{
    widgetData widget;
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    sceClibMemset(widget.parentRefId, 0, sizeof(widget.parentRefId));
    if(parentRefID != NULL) sceClibStrncpy(widget.parentRefId, parentRefID, sizeof(widget.parentRefId));

    widget.size = Size;
    widget.pos = Position;
    widget.type = button;
    widget.col = Color;

    sceClibMemset(widget.data.ButtonData.label, 0, sizeof(widget.data.ButtonData.label));
    sceClibStrncpy(widget.data.ButtonData.label, Text, sizeof(widget.data.ButtonData.label));

    widget.data.ButtonData.onPress = OnPress;

    return addWidget(&widget);
}

int QuickMenuRebornCheckBox(const char *refID, const char *parentRefID, vector4 Size, vector4 Position, widgetColor Color, void(*OnToggle)(int state))
{
    widgetData widget;
    
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    sceClibMemset(widget.parentRefId, 0, sizeof(widget.parentRefId));
    sceClibStrncpy(widget.parentRefId, parentRefID, sizeof(widget.parentRefId));

    widget.size = Size;
    widget.pos = Position;
    widget.type = check_box;
    widget.col = Color;

    widget.data.CheckBoxData.OnToggle = OnToggle;

    return addWidget(&widget);

}

int QuickMenuRebornText(const char *refID, const char *parentRefID, vector4 Size, vector4 Position, widgetColor Color, const char *Text)
{
    widgetData widget;
    
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    sceClibMemset(widget.parentRefId, 0, sizeof(widget.parentRefId));
    sceClibStrncpy(widget.parentRefId, parentRefID, sizeof(widget.parentRefId));

    widget.size = Size;
    widget.pos = Position;
    widget.col = Color;
    widget.type = text;

    sceClibMemset(widget.data.ButtonData.label, 0, sizeof(widget.data.ButtonData.label));
    sceClibStrncpy(widget.data.ButtonData.label, Text, sizeof(widget.data.ButtonData.label));

    return addWidget(&widget);

}

int QuickMenuRebornPlane(const char *refID, const char *parentRefID, vector4 Size, vector4 Position, widgetColor Color)
{
    widgetData widget;
    
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    sceClibMemset(widget.parentRefId, 0, sizeof(widget.parentRefId));
    sceClibStrncpy(widget.parentRefId, parentRefID, sizeof(widget.parentRefId));

    widget.size = Size;
    widget.pos = Position;
    widget.col = Color;
    widget.type = plane;

    return addWidget(&widget);

}

int QuickMenuRebornSeparator(const char *refID)
{
    char sepID[256] = {0};
    sceClibSnprintf(sepID, sizeof(sepID), "qm_reborn_%s_separator", refID);

    QuickMenuRebornPlane(sepID, NULL, makeWidgetVector4(825.0f,2.0f,0.0f,0.0f), makeWidgetVector4(0,0,0,0), COLOR_TRANSPARENT);

    return 0;
}

int QuickMenuRebornUpdateButton(const char *refID, vector4 Size, vector4 Position, widgetColor Color, const char *Text, void(*OnPress)(void), int flags)
{
    widgetData widget;
    
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    widget.size = Size;
    widget.pos = Position;
    widget.type = button;
    widget.col = Color;

    sceClibMemset(widget.data.ButtonData.label, 0, sizeof(widget.data.ButtonData.label));
    sceClibStrncpy(widget.data.ButtonData.label, Text, sizeof(widget.data.ButtonData.label));

    widget.data.ButtonData.onPress = OnPress;

    return updateWidget(&widget, flags);
}

int QuickMenuRebornUpdateCheckBox(const char *refID, vector4 Size, vector4 Position, widgetColor Color, void(*OnToggle)(int state), int flags)
{
    widgetData widget;
    
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));


    widget.size = Size;
    widget.pos = Position;
    widget.type = check_box;
    widget.col = Color;

    widget.data.CheckBoxData.OnToggle = OnToggle;

    return updateWidget(&widget, flags);
}

int QuickMenuRebornUpdateText(const char *refID, vector4 Size, vector4 Position, widgetColor Color, const char *Text, int flags)
{
    widgetData widget;
    
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    widget.size = Size;
    widget.pos = Position;
    widget.col = Color;
    widget.type = text;

    sceClibMemset(widget.data.ButtonData.label, 0, sizeof(widget.data.ButtonData.label));
    sceClibStrncpy(widget.data.ButtonData.label, Text, sizeof(widget.data.ButtonData.label));

    return updateWidget(&widget, flags);

}

int QuickMenuRebornUpdatePlane(const char *refID, vector4 Size, vector4 Position, widgetColor Color, int flags)
{
    widgetData widget;
    
    sceClibMemset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    widget.size = Size;
    widget.pos = Position;
    widget.col = Color;
    widget.type = plane;

    return updateWidget(&widget, flags);

}