#include <stdio.h>
#include "pluginloader.h"

#ifdef QUAB_OS_WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <dlfcn.h>
#endif

// define some static variable for control the function of this module
static bool HAS_CONFIG;

void* qbase_plugin_init(wchar_t* path)   {
    FILE* f = _wfopen(path, L"r+");
    HAS_CONFIG = f != NULL;
    if(HAS_CONFIG)  {
        // TODO: use the basescript function to load the DSL of config
    }
    return NULL;
}

void* qbase_plugin_load(char* pluginName, char *module)  {
    // TODO: use the basescript function to get the path by pluginName in lua's table
    #ifdef QUAB_OS_WIN32

    #else

    #endif

    return NULL;
}

void qbase_plugin_unload(void* module)  {
   // TODO: UNLOAD the module
    #ifdef QUAB_OS_WIN32

    #else

    #endif
}
