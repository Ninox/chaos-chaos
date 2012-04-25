#ifndef PLATFORM_H
#define PLATFORM_H

void qbase_plugin_init(wchar_t* path);
void* qbase_plugin_load(char* pluginName, char* module);
void  qbase_plugin_unload();

#endif // PLATFORM.h
