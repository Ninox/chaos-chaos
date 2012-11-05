#ifndef QUAB_LUA_ENV_H
#define QUAB_LUA_ENV_H

#include "QuabLuaBase.h"


namespace Quab  
{
    class QuabLuaTable;

    class QUAB_API QuabLuaEnv
    {
    private:
        lua_State *L;
        QuabLuaEnv();
    public:
        static QuabLuaEnv* createEnv(const char *name);
        static void releaseEnv(const char *name);

        void set(const char *name, const luaVariant &v);
        luaVariant get(const char *name);

        void load(const char *file);
        void load(const QuabStream *s);
        void exec(const char *file);
        void exec(const QuabStream *s);

        bool register_to(const char *name , quabLuaCallback f);
        bool call(const char *fname, const QuabLuaTable* para);
    };
}

#endif // QuabLuaEnv
