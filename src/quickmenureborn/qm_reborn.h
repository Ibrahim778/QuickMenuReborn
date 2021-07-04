#ifndef QM_REBORN_H
#define QM_REBORN_H

#include "c_types.h"

widgetColor makeWidgetColor(float r, float g, float b, float a);
vector4 makeWidgetVector4(float x, float y, float z, float w);

#define NOT_UPDATE (void*)0

//#ifdef __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

char *registerWidget(widgetData data);
int updateWidget(widgetData data, int flags);
int editWidget(widgetData data, int flags);
int unregisterWidget(char *refId);

int qm_reborn_ezcheckbox(char *refID, char *parentRefID, vector4 Size, vector4 Position, widgetColor Color, void (*onToggle)(int state));
int qm_reborn_eztext(char *refID, char *parentRefID, vector4 Size, vector4 Position, widgetColor Color, char *Text, int isBold);
int qm_reborn_ezbutton(char *refID,  char *parentRefID, vector4 Size, vector4 Position, widgetColor Color, char *Text, void (*onPress)(void));
int qm_reborn_ezseparator(char *refID);
int qm_reborn_ezseparator2(char *refID);
int qm_reborn_ezplane(char *refID, char *parentRefID, vector4 Size, vector4 Position, widgetColor Color);

int qm_reborn_ezupdate_text(char *refId, vector4 Size, vector4 Position, widgetColor Color, char *Text, int isBold, int flags);
int qm_reborn_ezupdate_checkbox(char *refID, vector4 Size, vector4 Position, widgetColor Color, void (*onToggle)(int state), int flags);
int qm_reborn_ezupdate_plane(char *refID, vector4 Size, vector4 Position, widgetColor Color, int flags);
int qm_reborn_ezupdate_button(char *refID, vector4 Size, vector4 Position, widgetColor Color, char *Text, void (*OnPress)(void), int flags);

#ifdef __cplusplus
}
#endif

/*
#else
char *_Z14registerWidget10widgetData(widgetData data);
__attribute__((used))
int _Z16unregisterWidgetPc(char *refId);
int _Z20qm_reborn_ezcheckboxPcS_7vector4S0_11widgetColorPFviE(char *refID, char *parentRefID, vector4 Size, vector4 Position, widgetColor Color, void (*onToggle)(int state));
int _Z18qm_reborn_ezbuttonPcS_7vector4S0_11widgetColorS_PFvvE(char *refID, char *parentRefID, vector4 Size, vector4 Position, widgetColor Color, char *Text, void(*onPress)(void));
int _Z16qm_reborn_eztextPcS_7vector4S0_11widgetColorS_(char *refID,  char *parentRefID, vector4 Size, vector4 Position, widgetColor Color, char *Text);

#define unregisterWidget _Z16unregisterWidgetPc
#define registerWidget _Z14registerWidget10widgetData
#define qm_reborn_ezcheckbox _Z20qm_reborn_ezcheckboxPcS_7vector4S0_11widgetColorPFviE
#define qm_reborn_eztext _Z16qm_reborn_eztextPcS_7vector4S0_11widgetColorS_
#define qm_reborn_ezbutton _Z18qm_reborn_ezbuttonPcS_7vector4S0_11widgetColorS_PFvvE
#define qm_reborn_ezseparator 
#define qm_reborn_ezplane 

#endif
*/
#endif