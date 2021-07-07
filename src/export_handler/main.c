#define QM_REBORN_INTERNAL
#include "../quickmenureborn/c_types.h"
#include <kernel.h>
#include <libdbg.h>

int module_start()
{
    SCE_DBG_LOG_INFO("Exporter Loaded!\n");
    return SCE_KERNEL_START_SUCCESS;
}

int module_stop()
{
    return SCE_KERNEL_STOP_SUCCESS;
}

int updateWidget(widgetData data, int flags)
{
    SceUID pipe = sceKernelOpenMsgPipe(EXPORT_PIPE);
    
    exportPacket packet;
    packet.data = data;
    packet.type = update_widget;
    packet.updateFlags = flags;
    
    SceSize sent = 0;
    if(sceKernelSendMsgPipe(pipe, &packet, sizeof(packet), SCE_KERNEL_MSG_PIPE_MODE_WAIT | SCE_KERNEL_MSG_PIPE_MODE_FULL, &sent, NULL) != SCE_OK)
    {
        sceKernelCloseMsgPipe(pipe);
        return -2;
    }
#ifdef DEBUG
    if(sent != sizeof(packet)) SCE_DBG_LOG_ERROR("Error couldn't send all the data!\n");
#endif
    return 0;
}

int addWidget(widgetData data)
{
    SCE_DBG_LOG_INFO("Adding export\n");
    SceUID pipe = sceKernelOpenMsgPipe(EXPORT_PIPE);
    
    exportPacket packet;
    packet.data = data;
    packet.type = register_widget;
    
    SceSize sent = 0;
    if(sceKernelSendMsgPipe(pipe, &packet, sizeof(packet), SCE_KERNEL_MSG_PIPE_MODE_WAIT | SCE_KERNEL_MSG_PIPE_MODE_FULL, &sent, NULL) != SCE_OK)
    {
        sceKernelCloseMsgPipe(pipe);
        return -2;
    }
#ifdef DEBUG
    if(sent != sizeof(packet)) SCE_DBG_LOG_ERROR("Error couldn't send all the data!\n");
#endif
    return 0;
}

int removeWidget(const char *refID)
{
    SceUID pipe = sceKernelOpenMsgPipe(EXPORT_PIPE);
    
    exportPacket packet;
    packet.refId = refID;
    packet.type = unregister_widget;
    
    SceSize sent = 0;
    if(sceKernelSendMsgPipe(pipe, &packet, sizeof(packet), SCE_KERNEL_MSG_PIPE_MODE_WAIT | SCE_KERNEL_MSG_PIPE_MODE_FULL, &sent, NULL) != SCE_OK)
    {
        sceKernelCloseMsgPipe(pipe);
        return -2;
    }
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