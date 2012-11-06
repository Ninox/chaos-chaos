#ifndef QUAB_LUA_TABLE_H
#define QUAB_LUA_TABLE_H

#include "QuabLuaBase.h"
#include <stddef.h>

namespace Quab  
{
	class QuabLuaKeyIterator;
	
	class QUAB_API QuabLuaTable
    {
    private:
        quabInnerTable *table;
        const char *tableName;
        QuabLuaTable(const QuabLuaTable &t){}
    public:
        QuabLuaTable(const char *name = NULL);
        ~QuabLuaTable();

		void add(const luaVariant &v);
		void add(luaTKey key, const luaVariant &v);
		luaVariant& operator[](unsigned idx);
		luaVariant& operator[](luaTKey key);

        unsigned getIndexLength() const;
        unsigned getKeyCount() const;
        QuabLuaKeyIterator* getKeys(int *cnt) const;
    };
}

#endif // QuabLuaTable
