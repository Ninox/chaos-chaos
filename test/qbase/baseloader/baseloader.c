#include "baseloader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef	QBASE_OS_LINUX
#include <dlfcn.h>
#else
#include <windows.h>
#endif
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define QBASE_PLUGINCONFIG "pluginlist.cf"

typedef struct qbase_plugin	{
	#ifdef QBASE_OS_LINUX
	void	*handle;
	#else
	HMODULE	 handle;
	#endif
	const char*	plugin_name;
} qbase_plugin;

typedef struct qbase_plugin_list	{
	qbase_plugin	data;
	struct qbase_plugin_list *next;
	struct qbase_plugin_list *prev;
} qbase_plugin_list;



//	a list for store the plugin
static qbase_plugin_list *plugin_list = NULL;

static qbase_plugin_list *
list_search(const char *name)	{
	qbase_plugin_list	*node = plugin_list;
	while(node != NULL)
	{
		if(strcmp(node->data.plugin_name, name) == 0)	{
			return node;
		}
		else node = node->next;
	}
	return NULL;
}

static qbase_plugin_list *
list_seekend()	{
	qbase_plugin_list *node = plugin_list;
	if(node != NULL)    {
        while(node->next != NULL)
            node = node->next;
	}
	return node;
}

static char *
list_getconfig(const char *key)	{
	lua_State *L = NULL;
	FILE *f = NULL;
	char *val = NULL;
	const char *retStr = NULL;
	f = fopen(QBASE_PLUGINCONFIG, "r+");
	if(f != NULL)	{
		fclose(f);	// if the file exists, close and release it
		L = luaL_newstate();
		lua_gc(L, LUA_GCSTOP, 0);
		if(luaL_dofile(L, QBASE_PLUGINCONFIG) == 0)	{
			lua_getglobal(L, key);
			if(lua_type(L, -1) != LUA_TNIL)	{
                retStr = lua_tostring(L, -1);
                val = (char*)malloc(sizeof(char) * (strlen(retStr)+1));
                memset(val, 0, strlen(retStr)+1);
				strcpy(val, retStr);
			}
			lua_pop(L, 1);
		}
		lua_gc(L, LUA_GCCOLLECT, 0);
		lua_close(L);
	}
	return val;
}

static void
list_freenode(qbase_plugin_list* node)	{
	// 1. free the dynamic library
	#ifdef QBASE_OS_LINUX
	dlclose(node->data.handle);
	#else
	FreeLibrary(node->data.handle);
	#endif
	// 2. free the whole node
	free(node);
}


void
qbase_loader_init(const char *name)	{
	qbase_plugin_list *node = NULL, *end_node = list_seekend();
	char *path;
	// checked exists
	node = list_search(name);
	if(node == NULL)	{
		node = (qbase_plugin_list*)malloc(sizeof(qbase_plugin_list));
		node->data.plugin_name = name;
		node->next = NULL;
		node->prev = end_node;
		if(end_node != NULL)    {
            end_node->next = node;
		}
		else    {
            plugin_list = node;
		}
	}
	path = list_getconfig(name);
	#ifdef QBASE_OS_LINUX
	node->data.handle = dlopen(path, RTLD_LAZY);
	#else
	node->data.handle = LoadLibrary(path);
	#endif
	// release the string on heap
	free(path);
}

void
qbase_loader_free(const char *name)	{
	qbase_plugin_list	*node = plugin_list;
	while(node != NULL)
	{
		if(strcmp(node->data.plugin_name, name) == 0)	{
			// if the node have no PREV and NEXT, it means that is a head node
			if(node->prev == NULL && node->next == NULL)	{
				list_freenode(node);
				plugin_list = NULL;
				break;
			}
			else	{
				if(node->prev != NULL)
					node->prev->next = node->next;
                else
                    plugin_list = node->next;
				if(node->next != NULL)
					node->next->prev = node->prev;
				list_freenode(node);
				break;
			}

		}
		else node = node->next;
	}
}

void*
qbase_loader_getf(const char *name, const char *fname)	{
	qbase_plugin_list *node = list_search(name);
	if(node == NULL)	{
		return NULL;
	}
	else	{
		if(node->data.handle == NULL)
			return NULL;
		#ifdef QBASE_OS_LINUX
		return dlsym(node->data.handle, fname);
		#else
		return GetProcAddress(node->data.handle, fname);
		#endif
	}
}

void
qbase_loader_destory()	{
	qbase_plugin_list *node = plugin_list, *next_node = NULL;
	while(node != NULL)
	{
		next_node = node->next;
		list_freenode(node);
		node = next_node;
	}
	plugin_list = NULL;
}
