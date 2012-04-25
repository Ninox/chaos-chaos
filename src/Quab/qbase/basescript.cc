#include "basescript.h"
extern "C"  {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

static lua_State* L = NULL;

ret_status qbase_script_init()    {
    if(L == NULL)   {
        L = luaL_newstate();
        luaopen_base(L);
        lua_openlib(L);
    }
}

ret_status qbase_script_execute(char* src, int retcount, void** ret)  {
    if(L == NULL)
        return -1;
    int src_len = strlen(src);
    luaL_loadbuffer(L, src, src_len, "line");
    if(retcount>0)  {
        ret = new void*[retcount];
    }
}

ret_status qbase_script_load(char* path)  {
    lua_dofile(L, path);
}

void qbase_script_reg(char* name, qbase_function f) {
    lua_register(L, name, f);
}

qbase_return qbase_script_get(char* valname)    {

}

qbase_return qbase_script_getfield(char* table, char* field)    {

}

void qbase_script_close()   {
    lua_close(L);
}
