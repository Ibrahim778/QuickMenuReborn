#ifndef TYPES_HPP_QM_REBORN
#define TYPES_HPP_QM_REBORN
#include <libdbg.h>
#include <kernel.h>
#include <appmgr.h>
#include <kernel/libkernel.h>
#include <paf.h>
#include "main.h"
#include "../quickmenureborn/qm_reborn.h"

extern "C" bool widgetsDisplayed();
extern "C" bool checkFileExist(const char *id);

class Utils
{
public:
    static Widget::Color MakeSceColor(float r, float g, float b, float a);
    static Widget::Color MakeSceColor(widgetColor col);
    static Widget::Color MakeSceColor(widgetColor *col);
    static SceFVector4 MakeSceVector4(SceFloat x, SceFloat y, SceFloat z, SceFloat w);
    static SceFVector4 MakeSceVector4(vector4 *vect);
    static SceFVector4 MakeSceVector4(vector4 vect);
    static Widget *GetChildByHash(Widget *parent, SceUInt32 hash);
    static Widget *FindWidget(SceUInt32 hash);
    static SceInt32 SetWidgetLabel(const char *text, Widget *widget);
    static SceUInt32 GetHashById(const char *id);
    static SceVoid SetWidgetColor(Widget *w, widgetColor *col);
    static SceVoid SetWidgetPosition(Widget *w, vector4 *pos);
    static SceVoid SetWidgetSize(Widget *w, vector4 *size);
    static SceVoid SetWidgetColor(Widget *w, float r, float g, float b, float a);
    static SceVoid SetWidgetPosition(Widget *wi, float x, float y, float z, float w);
    static SceVoid SetWidgetSize(Widget *wi, float x, float y, float z, float w);
};

#endif