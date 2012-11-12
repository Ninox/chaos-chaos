#include "lua/QuabLuaTable.h"
#include <stdlib.h>
using namespace Quab;

/*      helper functions        */
static void
luatable_releasevar(luaVariant &v)  {
    if(v.vartype == VAR_TABLE)  {
        delete v.value.table;
        v.value.table = NULL;
    }
}

/*      inner data structure definations    */
namespace Quab {
    struct quabInnerTable   {
        _luaTableArray_ array;
        _luaTableMap_ mapper;
    };
}

/*      class methods implemetns        */
QuabLuaTable::QuabLuaTable(const char *name)    {
    this->tableName = name;
    this->table = new quabInnerTable();
    this->table->array.reserve(30);
}

QuabLuaTable::~QuabLuaTable()   {
    if(this->table != NULL) {
        _luaTableArray_ &innerArray = this->table->array;
        _luaTableMap_ &innerMapper = this->table->mapper;
        /*  release all table variant in array or table*/
        _luaTableArray_::iterator aIter = innerArray.begin();
        _luaTableMap_::iterator mIter = innerMapper.begin();
        while(aIter != innerArray.end() || mIter != innerMapper.end())    {
            if(aIter != innerArray.end())   {
                // if the luaVariant is table, release
                luatable_releasevar((*aIter));
                aIter++;
            }
            if(mIter != innerMapper.end())  {
                luatable_releasevar((*mIter).second);
                mIter++;
            }
        }        
        /*      clear the array and table       */
        this->table->array.clear();
        this->table->mapper.clear();

        delete this->table;
    }
    this->table = NULL;
}

void QuabLuaTable::add(const luaVariant &v) {
    this->table->array.push_back(v);
}

void QuabLuaTable::add(luaTKey key, const luaVariant &v)    {
    if(this->table->mapper.count(key.key) > 0)  {
        luaVariant &mVar = this->table->mapper[key.key];
        if(mVar.vartype == VAR_TABLE) {
            delete mVar.value.table;
            mVar.value.table = NULL;
        }
        this->table->mapper[key.key] = v;
    } else  {
        this->table->mapper.insert(_luaTableMap_::value_type(key.key, v));
    }
}

luaVariant& QuabLuaTable::operator[](unsigned idx)  {
    return this->table->array[idx];
}

luaVariant& QuabLuaTable::operator[](luaTKey key)   {
    return this->table->mapper[key.key];
}

unsigned QuabLuaTable::getIndexLength() const   {
    return this->table->array.size();
}
unsigned QuabLuaTable::getKeyCount() const  {
    return this->table->mapper.size();
}
QuabLuaKeyIterator* QuabLuaTable::getKeys(int *cnt) const {
    *cnt = this->table->mapper.size();
    return new QuabLuaKeyIterator(this->table->mapper.begin(), this->table->mapper.end());
}
