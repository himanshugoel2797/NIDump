#ifndef _NIDUMP_FUNCTIONS_H_
#define _NIDUMP_FUNCTIONS_H_

#include <psp2/types.h>

int sceKernelGetModuleList(const SceUInt, SceUID *uid_array, SceUInt *numEntries);

#endif
