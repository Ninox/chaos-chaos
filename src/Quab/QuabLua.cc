#include "resource/QuabResource.h"
#include "Lua/QuabLua.h"
#include <lua.hpp>

using namespace Quab;

QuabLua::QuabLua()      {
      this->L = luaL_newstate();
      luaL_openlibs(this->L);
}

QuabLua::~QuabLua()     {
      if(this->L != NULL)     {
            lua_close(this->L);
            this->L = NULL;
      }
}

void QuabLua::set(const char *name, int value)  {
      lua_pushinteger(this->L, value);
      lua_setglobal(this->L, name);
}

void QuabLua::set(const char *name, double value)  {
      lua_pushnumber(this->L, value);
      lua_setglobal(this->L, name);
}

void QuabLua::set(const char *name, const char *str)  {
      lua_pushstring(this->L, str);
      lua_setglobal(this->L, name);
}

void QuabLua::set(const char *name, bool value)  {
      lua_pushboolean(this->L, value? 1 : 0);
      lua_setglobal(this->L, name);
}

void QuabLua::set(const char *name, const QuabLuaTable &value)  {

}

bool QuabLua::exists(const char *name, int *type)      {
      lua_getglobal(this->L, name);
      bool flag = false;
      if(lua_type(this->L, -1) != LUA_TNIL)     {
            flag = true;
      }
      lua_pop(this->L, 1);
      return flag;
}

QuabLuaValue QuabLua::get(const char *name)     {
      lua_getglobal(this->L, name);
      QuabLuaValue lv;
      lv.LuaValueType = Quab::VALUE_UNKNOW;
      switch(lua_type(this->L, -1)) {
            case LUA_TNIL:
                  lv.LuaValueType = Quab::VALUE_NIL;
                  lua_pop(this->L, 1);
                  break;
            case LUA_TNUMBER:
                  lv = QuabLuaValue(lua_tonumber(this->L, -1));
                  lua_pop(this->L, 1);
                  return lv;
            case LUA_TSTRING:
                  lv = QuabLuaValue(lua_tostring(this->L, -1));
                  lua_pop(this->L, 1);
                  break;
            case LUA_TBOOLEAN:
                  lv = QuabLuaValue(lua_toboolean(this->L, -1) == 1);
                  lua_pop(this->L, 1);
                  return lv;
            case LUA_TTABLE:
                  break;
            default:
                  break;
      }

      return lv;
}

bool QuabLua::exec(const QuabStream *buffer, bool isString)    {
      int stackCnt = 0;
      bool success;
      if(isString)      {
            success = luaL_dostring(this->L, buffer->getStream()) == 0;
      } else      {
            success = luaL_loadbuffer(this->L, buffer->getStream(), buffer->getSize(), "buffer_chunck") == 0;
            success = success && lua_pcall(this->L, 0, LUA_MULTRET, 0) == 0;            
      }
      stackCnt = lua_gettop(this->L);
      if(stackCnt > 0)  {
            lua_pop(this->L, stackCnt);
      }
      return success;
}

bool QuabLua::exec(const char *file)      {
      bool success = luaL_dofile(this->L, file) == 0;
      int stackCnt = lua_gettop(this->L) > 0;
      if(stackCnt > 0)
            lua_pop(this->L, stackCnt);
      return success;
}

bool QuabLua::registerTo(QuabLuaCallback f, const char *name)    {
      lua_register(this->L, name, f);
}

std::vector<QuabLuaValue> QuabLua::call(const char *fname, const std::vector<QuabLuaValue> &param)    {
      
}
