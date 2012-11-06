#ifndef QUAB_LUA_ENV_H
#define QUAB_LUA_ENV_H

#include "QuabLuaBase.h"


namespace Quab  
{
	class QuabStream;

    class QUAB_API QuabLuaEnv
    {
    private:
        lua_State *L;
        
    public:
		QuabLuaEnv();
		~QuabLuaEnv();
		
        void set(const char *name, const luaVariant &v);
        luaVariant get(const char *name);
		bool exists(const char *name);

        bool exec(const char *file);
        bool exec(const QuabStream *s);

        bool register_to(const char *name , quabLuaCallback f);
        bool call(const char *fname, const QuabLuaTable* para, int nargs = 0);
    };
}

#endif // QuabLuaEnv
