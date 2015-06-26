#ifndef  _NIDUMP_MODULE_UTILS_H_
#define  _NIDUMP_MODULE_UTILS_H_

#include "stdio.h"
#include <psp2/moduleinfo.h>
#include <psp2/io/fcntl.h>
#include <psp2/kernel/threadmgr.h>
#include "psp2/types.h"
#include "Functions.h"
#include "config.h"
#include "string.h"
#include "utils.h"
#include "menuAPI.h"

/* \brief SCE module export table
* Can be found in an ELF file or loaded in
* memory.
*/

#define MOD_INFO_VALID_ATTR     0x0000
#define MOD_INFO_VALID_VER      0x0101

#define MODULE_FILENAME_LENGTH 256
#define SEGMENT_COUNT 4

typedef struct module_exports // thanks roxfan
{
  SceUShort16   size;           // size of this structure; 0x20 for Vita 1.x
  SceUInt8    lib_version[2]; //
  SceUShort16   attribute;      // ?
  SceUShort16   num_functions;  // number of exported functions
  SceUInt   num_vars;       // number of exported variables
  SceUInt   num_tls_vars;   // number of exported TLS variables?  <-- pretty sure wrong // yifanlu
  SceUInt   module_nid;     // NID of this specific export list; one PRX can export several names
  char    *lib_name;      // name of the export module
  SceUInt   *nid_table;     // array of 32-bit NIDs for the exports, first functions then vars
  void    **entry_table;  // array of pointers to exported functions and then variables
} module_exports_t;

/**
* \brief SCE module import table (< 3.0 format)
*
* Can be found in an ELF file or loaded in
* memory.
*/
typedef struct // thanks roxfan
{
  SceUShort16   size;               // size of this structure; 0x34 for Vita 1.x
  SceUShort16   lib_version;        //
  SceUShort16   attribute;          //
  SceUShort16   num_functions;      // number of imported functions
  SceUShort16   num_vars;           // number of imported variables
  SceUShort16   num_tls_vars;       // number of imported TLS variables
  SceUInt   reserved1;          // ?
  SceUInt   module_nid;         // NID of the module to link to
  char    *lib_name;          // name of module
  SceUInt   reserved2;          // ?
  SceUInt   *func_nid_table;    // array of function NIDs (numFuncs)
  void    **func_entry_table; // parallel array of pointers to stubs; they're patched by the loader to jump to the final code
  SceUInt   *var_nid_table;     // NIDs of the imported variables (numVars)
  void    **var_entry_table;  // array of pointers to "ref tables" for each variable
  SceUInt   *tls_nid_table;     // NIDs of the imported TLS variables (numTlsVars)
  void    **tls_entry_table;  // array of pointers to ???
} module_imports_2x_t;

/**
* \brief SCE module import table (>= 3.x format)
*
* Can be found in an ELF file or loaded in
* memory.
*/
typedef struct
{
  SceUShort16   size;               // size of this structure; 0x24 for Vita 3.x
  SceUShort16   lib_version;        //
  SceUShort16   attribute;          //
  SceUShort16   num_functions;      // number of imported functions
  SceUShort16   num_vars;           // number of imported variables
  SceUShort16   unknown1;
  SceUShort16   module_nid;         // NID of the module to link to
  char    *lib_name;          // name of module
  SceUInt   *func_nid_table;    // array of function NIDs (numFuncs)
  void    **func_entry_table; // parallel array of pointers to stubs; they're patched by the loader to jump to the final code
  SceUInt   *var_nid_table;     // NIDs of the imported variables (numVars)
  void    **var_entry_table;  // array of pointers to "ref tables" for each variable
} module_imports_3x_t;

/**
* \brief SCE module import table
*/
typedef union module_imports
{
  SceUShort16 size;
  module_imports_2x_t old_version;
  module_imports_3x_t new_version;
} module_imports_t;

#define IMP_GET_NEXT(imp) ((module_imports_t *)((char *)imp + imp->size))
#define IMP_GET_FUNC_COUNT(imp) (imp->size == sizeof (module_imports_3x_t) ? imp->new_version.num_functions : imp->old_version.num_functions)
#define IMP_GET_VARS_COUNT(imp) (imp->size == sizeof (module_imports_3x_t) ? imp->new_version.num_vars : imp->old_version.num_vars)
#define IMP_GET_NID(imp) (imp->size == sizeof (module_imports_3x_t) ? imp->new_version.module_nid : imp->old_version.module_nid)
#define IMP_GET_NAME(imp) (imp->size == sizeof (module_imports_3x_t) ? imp->new_version.lib_name : imp->old_version.lib_name)
#define IMP_GET_FUNC_TABLE(imp) (imp->size == sizeof (module_imports_3x_t) ? imp->new_version.func_nid_table : imp->old_version.func_nid_table)
#define IMP_GET_FUNC_ENTRIES(imp) (imp->size == sizeof (module_imports_3x_t) ? imp->new_version.func_entry_table : imp->old_version.func_entry_table)
#define IMP_GET_VARS_TABLE(imp) (imp->size == sizeof (module_imports_3x_t) ? imp->new_version.var_nid_table : imp->old_version.var_nid_table)
#define IMP_GET_VARS_ENTRIES(imp) (imp->size == sizeof (module_imports_3x_t) ? imp->new_version.var_entry_table : imp->old_version.var_entry_table)


typedef struct
{
  SceUInt           size;   // this structure size (0x18)
  SceUInt           perms;  // probably rwx in low bits
  void            *vaddr; // address in memory
  SceUInt           memsz;  // size in memory
  SceUInt           flags;  // meanig unknown
  SceUInt           res;    // unused?
} segment_info_t;

typedef struct
{
  SceUInt           size;               // 0x1B8 for Vita 1.x
  SceUInt           handle;             // kernel module handle?
  SceUInt           flags;              // some bits. could be priority or whatnot
  char            module_name[28];
  SceUInt           unkn_28;
  void            *module_start;
  SceUInt           unkn_30;
  void            *module_stop;
  void            *exidx_start;
  void            *exidx_end;
  SceUInt           unkn_40;
  SceUInt           unkn_44;
  void            *tls_init_data;
  SceUInt           tls_init_size;
  SceUInt           tls_area_size;
  char            file_path[256];         //
  segment_info_t  segments[SEGMENT_COUNT];
  SceUInt           type;       // 6 = user-mode PRX?
} loaded_module_info_t;

int GetModuleList(SceUID *uids, SceUInt maxUIDs);

int DumpAllNIDs(SceSize size,void* outputFile);
int DumpAllModules(SceSize size, void* args);
SceModuleInfo* FindModuleInfo(loaded_module_info_t *loadedModule);

#endif
