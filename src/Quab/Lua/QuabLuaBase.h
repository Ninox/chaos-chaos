#ifndef QUAB_LUA_BASE_H
#define QUAB_LUA_BASE_H

#include "../QuabDef.h"

#define QUAB_MAX_LUAPOOL 64

#define _LKEY(key) luaTKey(key)

#define VAR_NIL 0
#define VAR_BOOLEAN 1
#define VAR_NUMBER 3
#define VAR_STRING 4
#define VAR_TABLE 5

struct lua_State;
namespace Quab
{
    struct quabInnerTable;

    struct luaVariant   {
        union   {
            Real number;
            const char *str;
            bool boolean;
            quabInnerTable *table;
        } value;
        char vartype;
    };
    
	struct luaTKey {
		const char *key;
		luaTKey(const char *k) { key = k; };
	};
	
    typedef int (*quabLuaCallback)(lua_State *L);
}
#endif // QuabLuaBase
