#ifndef DEFAULT_HANDLERS_H
#define DEFAULT_HANDLERS_H

#include <kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

void defaultCheckBoxSave(const char *id);
void defaultCheckBoxRecall(const char *id);
void defaultSliderSave(const char *id);
void defaultSliderRecall(const char *id);

#ifdef __cplusplus
}
#endif

#endif