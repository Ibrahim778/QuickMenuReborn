#ifndef EXPORT_H_INTERNAL_H
#define EXPORT_H_INTERNAL_H
#ifdef DEBUG
#define print sceClibPrintf
#else
#define print dummy
void dummy(const char *fmt, ...)
{
    if(fmt != fmt) return;
}
#endif
#endif