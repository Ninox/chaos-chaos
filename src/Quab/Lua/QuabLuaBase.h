#ifndef QUAB_LUA_BASE_H
#define QUAB_LUA_BASE_H

#include "../QuabDef.h"

#define QUAB_MAX_LUAPOOL 64

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
    
    typedef int (*quabLuaCallback)(lua_State *L);

}
#endif // QuabLuaBase
