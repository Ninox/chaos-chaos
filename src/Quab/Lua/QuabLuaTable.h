#ifndef QUAB_LUA_TABLE_H
#define QUAB_LUA_TABLE_H

#include "QuabLuaBase.h"
#include <stddef.h>
#include <map>
#include <vector>

typedef std::map<const char*, Quab::luaVariant> _luaTableMap_;
typedef std::vector<Quab::luaVariant> _luaTableArray_;

namespace Quab  
{
	class QuabLuaKeyIterator    {
        private:
            _luaTableMap_::iterator _iter;
            _luaTableMap_::iterator _end;
        public:
            QuabLuaKeyIterator(_luaTableMap_::iterator iter, _luaTableMap_::iterator end)    {
                this->_iter = iter;
                this->_end = end;
            }
            inline bool isEnd() {
                return this->_iter == this->_end;
            }
            inline const char* get()   {
                return (*_iter).first;
            }
            QuabLuaKeyIterator& operator ++(int)  {
                this->_iter++;
                return *this;
            }
    };
	
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
