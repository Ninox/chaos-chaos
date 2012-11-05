#include "lua/QuabLuaEnv.h"

using namespace Quab;

QuabLuaEnv* QuabLuaEnv::createEnv(const char *name)  {

}
void QuabLuaEnv::releaseEnv(const char *name)    {

}

void QuabLuaEnv::set(const char *name, const luaVariant &v) {

}
luaVariant QuabLuaEnv::get(const char *name)    {

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

}
bool QuabLuaEnv::call(const char *fname, const QuabLuaTable* para)  {

}
