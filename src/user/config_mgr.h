
#ifndef QM_REBORN_CONFIG_MGR_H
#define QM_REBORN_CONFIG_MGR_H

#include "../quickmenureborn/qm_reborn.h"
#include <kernel.h>
#include <paf.h>

#define CONFIG_SAVE_DIR "ur0:quickMenuRebornConfig/"
#define CHECKBOX_SAVE_DIR "ur0:quickMenuRebornConfig/CheckBox/"


int readCheckBoxState(const char *refID);
int saveCheckBoxState(const char *refID, CheckBoxState state); 

#endif