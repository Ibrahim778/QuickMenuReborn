#define QM_REBORN_INTERNAL
#include "../quickmenureborn/c_types.h"
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

int removeWidget(const char *refID)
{
    exportPacket packet;
	sceClibStrncpy(packet.refId, refID, sizeof(packet.refId));
    packet.type = unregister_widget;
    
    SceSize sent = 0;
    if(sceKernelSendMsgPipe(msgPipeUid, &packet, sizeof(packet), SCE_KERNEL_MSG_PIPE_MODE_DONT_WAIT | SCE_KERNEL_MSG_PIPE_MODE_FULL, &sent, NULL) != SCE_OK)
        return -2;
#ifdef DEBUG
    if(sent != sizeof(packet)) SCE_DBG_LOG_ERROR("Error couldn't send all the data!\n");
#endif
    return sent;
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