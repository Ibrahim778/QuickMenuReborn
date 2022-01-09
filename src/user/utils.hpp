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
extern "C" void dummyprint(const char *fmt, ...);

namespace Utils
{
    Widget::Color MakeSceColor(float r, float g, float b, float a);
    Widget::Color MakeSceColor(widgetColor col);
    Widget::Color MakeSceColor(widgetColor *col);
    SceFVector4 MakeSceVector4(SceFloat x, SceFloat y, SceFloat z, SceFloat w);
    SceFVector4 MakeSceVector4(vector4 *vect);
    SceFVector4 MakeSceVector4(vector4 vect);
    Widget *GetChildByHash(Widget *parent, SceUInt32 hash);
    Widget *FindWidget(SceUInt32 hash);
    SceInt32 SetWidgetLabel(const char *text, Widget *widget);
    SceUInt32 GetHashById(const char *id);
    SceVoid SetWidgetColor(Widget *w, widgetColor *col);
    SceVoid SetWidgetPosition(Widget *w, vector4 *pos);
    SceVoid SetWidgetSize(Widget *w, vector4 *size);
    SceVoid SetWidgetColor(Widget *w, float r, float g, float b, float a);
    SceVoid SetWidgetPosition(Widget *wi, float x, float y, float z, float w);
    SceVoid SetWidgetSize(Widget *wi, float x, float y, float z, float w);
    SceBool CreateTextureFromFile(graphics::Texture *tex, const char *file);
    SceVoid DeleteTexture(graphics::Texture *tex, bool deletePointer);
};

#endif