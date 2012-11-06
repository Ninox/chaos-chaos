#include "lua/QuabLuaEnv.h"
#include <map>
#include <lua.hpp>
using namespace Quab;
using namespace std;

/*   helper methods   */
#define AUTOPOP_COUNTER 32

inline void luahelper_autopop(lua_State *L)    {
    static int counter = 0;
    if(counter >= AUTOPOP_COUNTER)  {
        int cnt = lua_gettop(L);
        lua_pop(L, cnt);
        counter = 0;
    } else  {
        counter++;
    }
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

void QuabLuaEnv::set(const char *name, luaVariant v) {
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
//        lvar.table = lua_toboolean(this->L, -1) == 1;
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

void QuabLuaEnv::load(const char *file) {

}
void QuabLuaEnv::load(const QuabStream *s)  {

}
void QuabLuaEnv::exec(const char *file) {

}
void QuabLuaEnv::exec(const QuabStream *s)  {

}

bool QuabLuaEnv::register_to(const char *name , quabLuaCallback f)  {
    return false;
}
bool QuabLuaEnv::call(const char *fname, const QuabLuaTable* para)  {
    return false;
}
