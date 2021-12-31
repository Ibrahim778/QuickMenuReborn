
#ifndef MAIN_H_QM_REBORN
#define MAIN_H_QM_REBORN

#include <libdbg.h>
#include <kernel.h>
#include <appmgr.h>
#include <kernel/libkernel.h>
#include <paf.h>
#include <taihen.h>
#include <display.h>

using namespace paf;
using namespace ui;

#define PLUGINS_DIR2 "ur0:QuickMenuReborn/"
#define PLUGINS_DIR "ux0:QuickMenuReborn/"


int sceClibPrintf(const char * fmt, ...);

#define QM_REBORN_INTERNAL

#ifdef _DEBUG
#define print sceClibPrintf
#else
#define print dummyprint
#endif

#endif