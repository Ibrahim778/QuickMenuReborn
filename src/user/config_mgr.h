
#ifndef QM_REBORN_CONFIG_MGR_H
#define QM_REBORN_CONFIG_MGR_H

#include "../quickmenureborn/qm_reborn.h"
#include <kernel.h>
#include <paf/stdc.h>

#define CONFIG_SAVE_DIR "ur0:quickMenuRebornConfig/"
#define CHECKBOX_SAVE_DIR "ur0:quickMenuRebornConfig/CheckBox/"
#define SLIDEBAR_SAVE_DIR "ur0:quickMenuRebornConfig/Slidebar/"

int saveSlidebarState(const char *refId, SceFloat32 val);
SceFloat32 getSlidebarValue(const char *refID);
int readCheckBoxState(const char *refID);
int saveCheckBoxState(const char *refID, int state); 

#endif