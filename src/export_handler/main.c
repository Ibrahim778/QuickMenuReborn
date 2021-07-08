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

//Remove a previously added widget. It only reads the refID of the widget
int removeWidget(widgetData *data)
{
    exportPacket packet;
	sceClibMemcpy(&packet.data, data, sizeof(data));
    packet.type = unregister_widget;
    
    SceSize sent = 0;
    if(sceKernelSendMsgPipe(msgPipeUid, &packet, sizeof(packet), SCE_KERNEL_MSG_PIPE_MODE_DONT_WAIT | SCE_KERNEL_MSG_PIPE_MODE_FULL, &sent, NULL) != SCE_OK)
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

int QuickMenuRebornButton(const char *refID, const char *parentRefID, vector4 Size, vector4 Position, widgetColor Color, const char *Text, void(*OnPress)(void))
{
    widgetData widget;
    sce_paf_memset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    SCE_DBG_LOG_INFO("Set ref id to %s, original (from *) was %s\n", widget.refId, refID);

    sce_paf_memset(widget.parentRefId, 0, sizeof(widget.parentRefId));
    sceClibStrncpy(widget.parentRefId, parentRefID, sizeof(widget.parentRefId));

    widget.size = Size;
    widget.pos = Position;
    widget.type = button;
    widget.col = Color;

    sce_paf_memset(widget.data.ButtonData.label, 0, sizeof(widget.data.ButtonData.label));
    sceClibStrncpy(widget.data.ButtonData.label, Text, sizeof(widget.data.ButtonData.label));

    widget.data.ButtonData.onPress = OnPress;

    addWidget(&widget);
    return 0;
}

int QuickMenuRebornCheckBox(const char *refID, const char *parentRefID, vector4 Size, vector4 Position, widgetColor Color, void(*OnToggle)(int state))
{
    widgetData widget;
    
    sce_paf_memset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    sce_paf_memset(widget.parentRefId, 0, sizeof(widget.parentRefId));
    sceClibStrncpy(widget.parentRefId, parentRefID, sizeof(widget.parentRefId));

    widget.size = Size;
    widget.pos = Position;
    widget.type = check_box;
    widget.col = Color;

    widget.data.CheckBoxData.OnToggle = OnToggle;

    addWidget(&widget);
    return 0;
}

int QuickMenuRebornText(const char *refID, const char *parentRefID, vector4 Size, vector4 Position, widgetColor Color, const char *Text)
{
    widgetData widget;
    
    sce_paf_memset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    sce_paf_memset(widget.parentRefId, 0, sizeof(widget.parentRefId));
    sceClibStrncpy(widget.parentRefId, parentRefID, sizeof(widget.parentRefId));

    widget.size = Size;
    widget.pos = Position;
    widget.col = Color;
    widget.type = text;

    sce_paf_memset(widget.data.ButtonData.label, 0, sizeof(widget.data.ButtonData.label));
    sceClibStrncpy(widget.data.ButtonData.label, Text, sizeof(widget.data.ButtonData.label));

    addWidget(&widget);
    return 0;
}

int QuickMenuRebornPlane(const char *refID, const char *parentRefID, vector4 Size, vector4 Position, widgetColor Color)
{
    widgetData widget;
    
    sce_paf_memset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    sce_paf_memset(widget.parentRefId, 0, sizeof(widget.parentRefId));
    sceClibStrncpy(widget.parentRefId, parentRefID, sizeof(widget.parentRefId));

    widget.size = Size;
    widget.pos = Position;
    widget.col = Color;
    widget.type = plane;

    addWidget(&widget);
    return 0;
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
    
    sce_paf_memset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    widget.size = Size;
    widget.pos = Position;
    widget.type = button;
    widget.col = Color;

    sce_paf_memset(widget.data.ButtonData.label, 0, sizeof(widget.data.ButtonData.label));
    sceClibStrncpy(widget.data.ButtonData.label, Text, sizeof(widget.data.ButtonData.label));

    widget.data.ButtonData.onPress = OnPress;

    updateWidget(&widget, flags);
    return 0;
}

int QuickMenuRebornUpdateCheckBox(const char *refID, vector4 Size, vector4 Position, widgetColor Color, void(*OnToggle)(int state), int flags)
{
    widgetData widget;
    
    sce_paf_memset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));


    widget.size = Size;
    widget.pos = Position;
    widget.type = check_box;
    widget.col = Color;

    widget.data.CheckBoxData.OnToggle = OnToggle;

    updateWidget(&widget, flags);
    return 0;
}

int QuickMenuRebornUpdateText(const char *refID, vector4 Size, vector4 Position, widgetColor Color, const char *Text, int flags)
{
    widgetData widget;
    
    sce_paf_memset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    widget.size = Size;
    widget.pos = Position;
    widget.col = Color;
    widget.type = text;

    sce_paf_memset(widget.data.ButtonData.label, 0, sizeof(widget.data.ButtonData.label));
    sceClibStrncpy(widget.data.ButtonData.label, Text, sizeof(widget.data.ButtonData.label));

    updateWidget(&widget, flags);
    return 0;
}

int QuickMenuRebornUpdatePlane(const char *refID, vector4 Size, vector4 Position, widgetColor Color, int flags)
{
    widgetData widget;
    
    sce_paf_memset(widget.refId, 0, sizeof(widget.refId));
    sceClibStrncpy(widget.refId, refID, sizeof(widget.refId));

    widget.size = Size;
    widget.pos = Position;
    widget.col = Color;
    widget.type = plane;

    updateWidget(&widget, flags);
    return 0;
}