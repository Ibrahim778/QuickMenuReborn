
#ifndef MAIN_H_QM_REBORN
#define MAIN_H_QM_REBORN
#include <libdbg.h>
#include <kernel.h>
#include <appmgr.h>
#include <kernel/libkernel.h>
#include <paf.h>

using namespace paf;
using namespace widget;

#define QM_REBORN_INTERNAL

#ifdef DEBUG

#define TRY(method) do { sceClibPrintf("Trying " #method "\n"); void *ret = (void *)method; sceClibPrintf("Got ret = 0x%X\n", ret); } while(0);
#define TRY_RET(method, toSet, type) do { sceClibPrintf("Trying " #method "\n"); void *ret = (void *)method; sceClibPrintf("Got ret = 0x%X\n", ret); toSet = (type)ret;  } while(0)
#define DO(method) sceClibPrintf("Doing "#method "\n");
#define FAIL_IF(con) if (con) { sceClibPrintf("Error occoured! (%s got error)\n", #con); sceKernelDelayThread(3 * 1000000); return 0; } else sceClibPrintf("Check PASSED (%s)\n", #con);
#define FAILTHREAD_IF(con) if (con) { sceClibPrintf("Error occoured! (%s got error)\n", #con); sceKernelDelayThread(3 * 1000000); return sceKernelExitDeleteThread(0); } else sceClibPrintf("Check PASSED (%s)\n", #con);
#define NULL_ERROR_FAIL(con) if (con < 0 || con == NULL) { sceClibPrintf("Error occoured! (%s)\n", #con); sceKernelDelayThread(3 * 1000000); return -1; } else sceClibPrintf("Check PASSED (%s not null or error)\n", #con);

#else
#define TRY(method) 
#define TRY_RET(method, toSet, type) do { void *ret = (void *)method; toSet = (type)ret;  } while(0)
#define DO(method)
#define FAIL_IF(con) if (con) {  return 0; }
#define FAILTHREAD_IF(con) if (con) { return sceKernelExitDeleteThread(0); }
#define NULL_ERROR_FAIL(con) if (con < 0 || con == NULL) { return -1; }
#endif

#endif