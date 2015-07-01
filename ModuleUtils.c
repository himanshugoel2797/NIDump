#include "ModuleUtils.h"


SceModuleInfo* FindModuleInfo(loaded_module_info_t *loadedModule)
{
  SceModuleInfo *moduleInfo = NULL;
  void *location = loadedModule->segments[0].vaddr;
  SceUInt size = loadedModule->segments[0].memsz;

  //Find the module info string in this memory region
  while(size > 0)
  {
    location = boyer_moore(location, size, loadedModule->module_name, strlen(loadedModule->module_name));
    if(location == NULL)
    {
      printf("Failed to find module info");
      break;
    }

    //Perform a detailed check to make sure this is a real match
    moduleInfo = (SceModuleInfo*)((SceUInt)location - 4);
    if(moduleInfo->attr == MOD_INFO_VALID_ATTR && moduleInfo->ver == MOD_INFO_VALID_VER)break;
    else {
      moduleInfo = NULL;  //If check fails, this was a false positive and move on
      size -= ((SceUInt)location - (SceUInt)loadedModule->segments[0].vaddr) + strlen(loadedModule->module_name);
      location = ((SceUInt)location) + strlen(loadedModule->module_name);
    }
  }

  return moduleInfo;
}

int GetModuleList(SceUID *uids, SceUInt maxUIDs)
{
  SceUInt numEntries = maxUIDs;

  SceUInt errorCode = sceKernelGetModuleList(0xFF, uids, &numEntries);
  if(errorCode < 0){
    printf("Failed to get module list %i\n", errorCode);
    return -1;
  }

  printf("Num Entries: %d",numEntries);
  return numEntries;
}

int DumpAllNIDs(char *outputFile)
{
  menuStatusAppendBufferData("Dumping NIDs...\n");

  SceUID fp = sceIoOpen((char*)outputFile, PSP2_O_WRONLY | PSP2_O_CREAT, 0777);

  if(fp < 0){
    printf("Failed to create file for output: '%s'", outputFile);
    return -1;
  }

  SceUID moduleUIDs[MAX_LOADED_MODS];
  int numEntries = GetModuleList(moduleUIDs, MAX_LOADED_MODS);

  if(numEntries < 0){
    sceIoClose(fp);
    return -1;
  }

  for(int i = 0; i < numEntries; i++)
  {
    loaded_module_info_t m_info;
    m_info.size = sizeof(loaded_module_info_t);
    if(sceKernelGetModuleInfo(moduleUIDs[i], &m_info) < 0){
      printf("Failed to get module info (UID:%d)", moduleUIDs[i]);
    }else{

      SceModuleInfo *m_moduleInfo = FindModuleInfo(&m_info);

      printf("Module Name: %s", m_moduleInfo->name);
      sceIoWrite(fp, m_info.module_name, 28);
      sceIoWrite(fp, "\n", 1);
    }
  }

  sceIoClose(fp);
  menuStatusAppendBufferData("Done!\n");
  sceKernelExitDeleteThread(0);
}

int DumpAllModules()
{
  menuStatusAppendBufferData("Dumping Modules...\n");

  SceUID moduleUIDs[MAX_LOADED_MODS];
  int numEntries = GetModuleList(moduleUIDs, MAX_LOADED_MODS);

  if(numEntries < 0)return -1;

  for(int i = 0; i < numEntries; i++)
  {
    loaded_module_info_t m_info;
    m_info.size = sizeof(loaded_module_info_t);
    if(sceKernelGetModuleInfo(moduleUIDs[i], &m_info) < 0){
      printf("Failed to get module info (UID:%d)", moduleUIDs[i]);
    }else{

      SceModuleInfo *m_moduleInfo = FindModuleInfo(&m_info);
      if(m_moduleInfo != NULL){
        printf("Module Name: %s", m_moduleInfo->name);

        for(int segment_index = 0; segment_index < SEGMENT_COUNT; segment_index++){
          char filename[MODULE_FILENAME_LENGTH];
          strcpy(filename, "pss0:/top/Documents/");
          strcat(filename, m_moduleInfo->name);
          char int_val[2];
          sprintf(int_val, "_%d", segment_index);
          strcat(filename, int_val);
          strcat(filename, ".bin");

          printf("Output File: %s", filename);

          SceUID fp = sceIoOpen(filename, PSP2_O_CREAT | PSP2_O_WRONLY, 0777);
          if(fp < 0){
            printf("Error failed to open file \"%s\" for writing", filename);
          }
          sceIoWrite(fp, m_info.segments[segment_index].vaddr, m_info.segments[segment_index].memsz);
          sceIoClose(fp);
        }
      }
    }
  }
  menuStatusAppendBufferData("Done!\n");
  sceKernelExitDeleteThread(0);
}
