#include "Lua/QuabLua.h"
#include <lua.hpp>

using namespace Quab;

QuabLua::QuabLua()      {
      this->L = luaL_newstate();
}

QuabLua::~QuabLua()     {
      if(this->L != NULL)     {
            lua_close(this->L);
            this->L = NULL;
      }
}

void QuabLua::set(const char *name, int value)  {

}

void QuabLua::set(const char *name, double value)  {

}

void QuabLua::set(const char *name, const char *str)  {

}

void QuabLua::set(const char *name, bool value)  {

}

void QuabLua::set(const char *name, const QuabLuaTable &value)  {

}

bool QuabLua::exists(const char *name, int *type)      {
}

QuabLuaValue QuabLua::get(const char *name)     {

}

bool QuabLua::load(const QuabStream *buffer)    {

}

bool QuabLua::load(const char * path)     {

}

void QuabLua::exec(const QuabStream *buffer)    {

}

void QuabLua::exec(const char *file)      {
      
}

bool QuabLua::registerTo(QuabLuaCallback f, const char *name)    {

}

void QuabLua::call(const char *fname, const std::vector<QuabLuaValue> &param)    {

}
