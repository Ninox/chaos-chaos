#ifndef PLATFORM_H
#define PLATFORM_H

void qbase_plugin_init(wchar_t* path);
void qbase_plugin_load(char* pluginName, void* module);
void qbase_plugin_unload(void* module);

#endif // PLATFORM.h