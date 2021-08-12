#include "config_mgr.h"

int checkFileExist(const char *path)
{
	SceIoStat stat;
	return sceIoGetstat(path, &stat) >= 0;
}

int readIntFromFile(const char *path)
{
	SceUID file = sceIoOpen(path, SCE_O_RDONLY, 0777);
	int ret = 0;
	sceIoRead(file, &ret, sizeof(int));
	sceIoClose(file);
	return ret;
}

int writeIntToFile(const char *path, int val)
{
	SceUID file = sceIoOpen(path, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);
	int ret = sceIoWrite(file, &val, sizeof(val));
	sceIoClose(file);
	return ret == sizeof(int) ? CONFIG_MGR_OK : CONFIG_MGR_ERROR_COULD_NOT_WRITE_FULL;
}

int preSetup()
{
	int ret = 0;
	if(!checkFileExist(CONFIG_SAVE_DIR))
	{
		sceIoMkdir(CONFIG_SAVE_DIR, 0777);
		sceIoMkdir(CHECKBOX_SAVE_DIR, 0777);
	}
	else if(!checkFileExist(CHECKBOX_SAVE_DIR))
		sceIoMkdir(CHECKBOX_SAVE_DIR, 0777);
	
	return ret;
}

int readCheckBoxState(const char *refID)
{
	int ret = 0;
	if(ret = preSetup(), ret < 0) return ret;

	char key[0x400] = {0};

	sce_paf_snprintf(key, 0x400, CHECKBOX_SAVE_DIR "%s", refID);

	if(!checkFileExist(key)) return CONFIG_MGR_ERROR_NOT_EXIST;

	ret = readIntFromFile(key);

	return !ret;
}

int saveCheckBoxState(const char *refID, CheckBoxState val)
{
	int ret = CONFIG_MGR_OK;
	if(ret = preSetup(), ret < 0) return ret;

	char key[0x400] = {0};
	sce_paf_snprintf(key, 0x400, CHECKBOX_SAVE_DIR "%s", refID);
	
	ret = writeIntToFile(key, val);

	return ret;
}