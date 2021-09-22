
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
using namespace widget;

#define PLUGINS_DIR2 "ur0:QuickMenuReborn/"
#define PLUGINS_DIR "ux0:QuickMenuReborn/"


int sceClibPrintf(const char * fmt, ...);
#define QM_REBORN_INTERNAL
#ifdef _DEBUG
#define print sceClibPrintf
#define TRY(method) do { sceClibPrintf("Trying " #method "\n"); void *ret = (void *)method; sceClibPrintf("Got ret = 0x%X\n", ret); } while(0);
#define TRY_RET(method, toSet, type) do { sceClibPrintf("Trying " #method "\n"); type ret = (type)method; sceClibPrintf("Got ret = 0x%X\n", ret); toSet = ret;  } while(0)
#define DO(method) sceClibPrintf("Doing "#method "\n");
#define FAIL_IF(con) if (con) { sceClibPrintf("Error occoured! (%s got error)\n", #con);  return 0; } else sceClibPrintf("Check PASSED (%s)\n", #con);
#define FAILTHREAD_IF(con) if (con) { sceClibPrintf("Error occoured! (%s got error)\n", #con);  return sceKernelExitDeleteThread(0); } else sceClibPrintf("Check PASSED (%s)\n", #con);
#define NULL_ERROR_FAIL(con) if (con < 0 || con == NULL) { sceClibPrintf("Error occoured! (%s)\n", #con); return -1; } else sceClibPrintf("Check PASSED (%s not null or error)\n", #con);

#else
#define print dummyprint
#define TRY(method) method;
#define TRY_RET(method, toSet, type) do { type ret = (type)method; toSet = (type)ret;  } while(0)
#define DO(method)
#define FAIL_IF(con) if (con) {  return 0; }
#define FAILTHREAD_IF(con) if (con) { return sceKernelExitDeleteThread(0); }
#define NULL_ERROR_FAIL(con) if (con < 0 || con == NULL) { return -1; }
#endif

#endif