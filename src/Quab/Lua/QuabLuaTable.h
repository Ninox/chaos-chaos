#ifndef QUAB_LUA_TABLE_H
#define QUAB_LUA_TABLE_H

#include "QuabLuaBase.h"

namespace Quab  
{
    class QUAB_API QuabLuaTable
    {
    private:
        quabInnerTable *table;
        const char *tableName;
        QuabLuaTable(const QuabLuaTable &t){}
    public:
        QuabLuaTable(const char *name = NULL)  { this->table = NULL; this->tableName = name;}
        QuabLuaTable(const char *name = NULL, quabInnerTable *tb) { this->table = tb; this->tableName = name; }
        ~QuabLuaTable();

        void seti(unsigned idx, const luaVariant &v);
        void setk(const char *key, const luaVariant &v);
        luaVariant geti(unsigned idx);
        luaVariant getk(const char *key);

        unsigned getIndexLength() const;
        unsigned getKeyCount() const;
        char* getKeys(int *cnt) const;
    };
}

#endif // QuabLuaTable
