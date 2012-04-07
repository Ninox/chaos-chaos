#ifdef WIN32
    #include <windows.h>
#else

#endif
#include <stdio.h>
#include "pluginloader.h"

// define some static variable for control the function of this module
static bool HAS_CONFIG;

void qbase_plugin_init(wchar_t* path)   {
    FILE* f = _wfopen(path, L"r");
    HAS_CONFIG = f != NULL;
    if(HAS_CONFIG)  {
        // TODO: use the basescript function to load the DSL of config
    }
}

void qbase_plugin_load(char* pluginName, void* module)  {
    // TODO: use the basescript function to get the path by pluginName in lua's table
}

void qbase_plugin_unload(void* module)  {
   // TODO: UNLOAD the module
}
