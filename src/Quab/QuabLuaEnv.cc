#include "lua/QuabLuaEnv.h"
#include "resource/QuabResource.h"
#include <map>
#include <string.h>
#include <lua.hpp>
using namespace Quab;
using namespace std;

/*   helper methods   */
#define AUTOPOP_COUNTER 32

/*      helper functions        */
static void 
luahelper_autopop(lua_State *L)    {
    static int counter = 0;
    if(counter >= AUTOPOP_COUNTER)  {
        lua_pop(L, lua_gettop(L));
        counter = 0;
    } else  {
        counter++;
    }
}

static void
luahelper_tostack(lua_State *L, const QuabLuaTable &tb)    {

}

static QuabLuaTable *
luahelper_fromstack(lua_State *L, int idx)    {
}

/*      public class implements     */
QuabLuaEnv::QuabLuaEnv()  {
    this->L = luaL_newstate();
    luaL_openlibs(this->L);
}
QuabLuaEnv::~QuabLuaEnv()    {
    lua_close(this->L);
    this->L = NULL;
}

void QuabLuaEnv::set(const char *name, const luaVariant &v) {
    bool isContinue = true;
    switch(v.vartype)
    {
    case VAR_BOOLEAN:
        lua_pushboolean(this->L, v.value.boolean);
        break;
    case VAR_NUMBER:
        lua_pushnumber(this->L, v.value.number);
        break;
    case VAR_STRING:
        lua_pushstring(this->L, v.value.str);
        break;
    case VAR_TABLE:
    default:
        isContinue = false;
        break;
    }
    if(!isContinue)
        return;
    lua_setglobal(this->L, name);
}

luaVariant QuabLuaEnv::get(const char *name)    {
    luaVariant lvar;
    lua_getglobal(this->L, name);
    switch(lua_type(this->L, -1))
    {
    case LUA_TBOOLEAN:
        lvar.vartype = LUA_TBOOLEAN;
        lvar.value.boolean = lua_toboolean(this->L, -1) == 1;
        break;
    case LUA_TNUMBER:
        lvar.vartype = LUA_TNUMBER;
        lvar.value.number = lua_tonumber(this->L, -1);
        break;
    case LUA_TSTRING:
        lvar.vartype = LUA_TBOOLEAN;
        lvar.value.str = lua_tostring(this->L, -1);
        break;
    case LUA_TTABLE:
        lvar.vartype = LUA_TTABLE;
        lvar.value.table = luahelper_fromstack(this->L, -1);
        break;
    case LUA_TNIL:
    default:
        lvar.vartype = LUA_TNIL;
        break;
    }

    luahelper_autopop(this->L);
    return lvar;
}

bool QuabLuaEnv::exists(const char *name)   {
    if(name != NULL)    {
        lua_getglobal(this->L, name);
        bool isNIL = lua_type(this->L, -1) == LUA_TNIL;
        lua_pop(this->L, 1);
        return isNIL;
    }
    return false;
}

bool QuabLuaEnv::exec(const char *file) {
    if(file == NULL)
        return false;
    int ret = luaL_dofile(this->L, file);
    luahelper_autopop(this->L);
    return ret == 0;
}

bool QuabLuaEnv::exec(const QuabStream *s)  {
    if(s == NULL || s->getSize() == 0)
        return false;;
    luaL_loadbuffer(this->L, s->getStream(), s->getSize(), "buffer_chunk");
    int ret = lua_pcall(this->L, 0, LUA_MULTRET, 0);
    luahelper_autopop(this->L);
    return ret == 0;
}

bool QuabLuaEnv::register_to(const char *name , quabLuaCallback f)  {
    if(name == NULL || f == NULL)   {
        return false;
    }   else    {
        lua_register(this->L, name, f);
        return true;
    }
}
bool QuabLuaEnv::call(const char *fname, const QuabLuaTable* para, int nargs)  {
    char retVariant[50];
    lua_getglobal(this->L, fname);
    if(lua_type(this->L, -1) != LUA_TFUNCTION)  {
        lua_pop(this->L, 1);
        return false;
    }
    luahelper_tostack(this->L, (*para));
    lua_call(this->L, nargs, 1);
    strcpy(retVariant, fname);
    strcat(retVariant, "_results");
    lua_setglobal(this->L, retVariant);
    return true;
}
