#ifndef TYPES_H_QM_REBORN
#define TYPES_H_QM_REBORN
#include <libdbg.h>
#include <kernel.h>
#include <appmgr.h>
#include <kernel/libkernel.h>
#include <paf.h>
#include "main.h"
#include "../quickmenureborn/qm_reborn.h"

WString makeWString(const char *string);
Widget::Color makeSceColor(float r, float g, float b, float a);
Widget::Color makeSceColor(widgetColor col);
SceFVector4 makeSceVector4(SceFloat x, SceFloat y, SceFloat z, SceFloat w);
SceFVector4 makeSceVector4(vector4 *vect);
SceFVector4 makeSceVector4(vector4 vect);

#endif