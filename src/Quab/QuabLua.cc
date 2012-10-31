#include "resource/QuabResource.h"
#include "Lua/QuabLua.h"
#include <lua.hpp>

using namespace Quab;

/*    helper methods    */
static void luahelper_pushdata(lua_State *L, const QuabLuaObject &obj);  // predeclare the function
static void luahelper_getdata(lua_State *L, QuabLuaTable *table);

static bool
luahelper_checkexist(lua_State *L, const char *name)    {
      lua_getglobal(L, name);
      bool success;
      if(lua_type(L, -1) == LUA_TNIL)
          success = false;
      else success = true;
      lua_pop(L, 1);
      return success;
}

inline static bool
luahelper_checkgetter(lua_State *L, const char *name, int type) {
    lua_getglobal(L, name);
    return type == lua_type(L, -1);
}

static void
luahelper_pushtable(lua_State *L, const QuabLuaTable *table)    {
    // create a new table
    lua_newtable(L);
    // get array data
    int i = 1;
    const std::vector<QuabLuaObject> *_array = table->getArray();
    std::vector<QuabLuaObject>::const_iterator citer;
    for(citer = _array->begin(); citer != _array->end(); citer++) {
        luahelper_pushdata(L, (*citer));
        lua_rawseti(L, -2, i++);
    }
    // get key-value data
    const char *key = NULL;
    const QuabLuaObject *obj = NULL;
    const std::map<const char*, QuabLuaObject> *objMap = table->getMapper();
    std::map<const char*, QuabLuaObject>::const_iterator mapIter;
    for(mapIter = objMap->begin(); mapIter != objMap->end(); mapIter++)   {
        // push key
        lua_pushstring(L, (*mapIter).first);
        // push data
        luahelper_pushdata(L, (*mapIter).second);
        // set table
        lua_settable(L, -3);
    }
}

static void
luahelper_pushdata(lua_State *L, const QuabLuaObject &obj)  {
    switch(obj.type)
    {
    case Quab::QUAB_LUATYPE_NIL:
        lua_pushnil(L);
        break;
    case Quab::QUAB_LUATYPE_NUMBER:
        lua_pushnumber(L, obj.value.number);
        break;
    case Quab::QUAB_LUATYPE_STRING:
        lua_pushstring(L, obj.value.str);
        break;
    case Quab::QUAB_LUATYPE_BOOLEAN:
        lua_pushboolean(L, obj.value.boolean);
        break;
    case Quab::QUAB_LUATYPE_TABLE:
        luahelper_pushtable(L, obj.value.table);
        break;
    default:
        break;
    }
}

static void
luahelper_gettable(lua_State *L, QuabLuaTable *table)   {

}

static void
luahelper_getdata(lua_State *L, QuabLuaTable *table)    {

}

/*    the impletement of the QuabLuaTable    */
QuabLuaTable::QuabLuaTable()    {
    this->_luaArray.reserve(10);
}
QuabLuaTable::~QuabLuaTable()   {
    this->_luamap.clear();
    this->_luaArray.clear();
    this->_luaArray.reserve(1);
}

QuabLuaObject& QuabLuaTable::operator[](unsigned idx)   {
	idx--;
    if(idx >= this->_luaArray.size())
        throw "over the max index of Lua array";
    else 
        return this->_luaArray[idx];
}

QuabLuaObject& QuabLuaTable::operator[](const char *name)   {
    return this->_luamap[name];
}

void QuabLuaTable::add(const QuabLuaObject &v) {
    this->_luaArray.push_back(v);
}

void QuabLuaTable::add(const char *key, const QuabLuaObject &v) {
    this->_luamap.insert(std::map<const char*, QuabLuaObject>::value_type(key, v));
}

unsigned QuabLuaTable::getLength()  {
    return this->_luaArray.size();
}

QuabLuaKeyString* QuabLuaTable::getKeys(unsigned *cnt)   {
    unsigned counter = 0;
    std::map<const char*, QuabLuaObject>::const_iterator iter;
    QuabLuaKeyString *keyArray = (QuabLuaKeyString*)malloc(sizeof(QuabLuaKeyString) * this->_luamap.size());
    for(iter = this->_luamap.begin(); iter != this->_luamap.end(); iter++)  {
        keyArray[counter] = (*iter).first;
        counter++;
    }
    *cnt = counter;
    return keyArray;
}

/*    the impletement of QuabLua    */
static std::map<const char*, QuabLua*> __luaPool;

QuabLua::~QuabLua()     {
    this->L = NULL;
}

QuabLua* QuabLua::create(const char *vmName)  {    
    // search the vm in map
    if(__luaPool.count(vmName) > 0)    {
        return __luaPool[vmName];
    } else  {
        lua_State *L = luaL_newstate();
        luaL_openlibs(L);
        QuabLua *lua = new QuabLua();
        lua->L = L;
        __luaPool[vmName] = lua;
        return lua;
    }
}

void QuabLua::release(const char *vmName) {
    if(__luaPool.count(vmName) > 0) {
        __luaPool.erase(vmName);
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
    luahelper_pushtable(this->L, &value);
    lua_setglobal(this->L, name);
}

bool QuabLua::exists(const char *name)      {
     return luahelper_checkexist(this->L, name);
}

double QuabLua::getNumber(const char *name)  {
    lua_getglobal(this->L, name);
    double value = lua_tonumber(this->L, -1);
    lua_pop(this->L, 1);
    return value;
}

const char* QuabLua::getString(const char *name)    {
    lua_getglobal(this->L, name);
    const char *s = lua_tostring(this->L, -1);
    lua_pop(this->L, 1);
    return s;
}

bool QuabLua::getBoolean(const char *name)    {
    lua_getglobal(this->L, name);
    int boolean = lua_toboolean(this->L, -1);
    bool value = (boolean == 1);
    lua_pop(this->L, 1);
    return value;
}

QuabLuaTable* QuabLua::getTable(const char *name)    {
    QuabLuaTable *table = new QuabLuaTable();

    return table;
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
      unsigned popcnt = lua_gettop(this->L);
      if(popcnt > 0)
        lua_pop(this->L, popcnt);
      return success;
}

bool QuabLua::exec(const char *file)      {
    bool success = luaL_dofile(this->L, file) == 0;
    unsigned popcnt = lua_gettop(this->L);
    if(popcnt > 0)
        lua_pop(this->L, popcnt);
    return success;
}

bool QuabLua::registerTo(QuabLuaCallback f, const char *name)    {
      lua_register(this->L, name, f);
}

void QuabLua::call(const char *fname, const QuabLuaTable &para)   {
      
}
