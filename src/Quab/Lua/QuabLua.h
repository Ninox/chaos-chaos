#ifndef QUAB_LUA_H
#define QUAB_LUA_H

#include <vector>
#include <map>
#include <stdlib.h>
#include <stddef.h>
#include <string>
#include "../QuabDef.h"

#define QUAB_LUA_AUTOPOP 23

struct lua_State;

namespace Quab
{
    class QuabStream;
    class QuabLuaTable;
    typedef int (*QuabLuaCallback)(lua_State *L);
    typedef const char* QuabLuaKeyString; 
	
    enum QuabLuaType
    {
        QUAB_LUATYPE_NIL         = 0,
        QUAB_LUATYPE_BOOLEAN	 = 1,
        QUAB_LUATYPE_NUMBER		 = 3,
        QUAB_LUATYPE_STRING		 = 4,
        QUAB_LUATYPE_TABLE		 = 5
    };

    struct QuabLuaObject 
    {
        union   {
            double number;
            bool boolean;
            const char* str;
            QuabLuaTable *table;
        } value;

        int type;
    };

	class QUAB_API QuabLuaTable
	{
	private:
        std::vector<QuabLuaObject> _luaArray;        
        std::map<const char*, QuabLuaObject> _luamap;
        QuabLuaTable(const QuabLuaTable &table){}

	public:
        QuabLuaTable();
        ~QuabLuaTable();

        const std::map<const char*, QuabLuaObject>* getMapper() const { return &this->_luamap; }
        const std::vector<QuabLuaObject>* getArray() const { return &this->_luaArray; }

        QuabLuaObject& operator[](unsigned idx);
        QuabLuaObject& operator[](const char *name);
        void add(const QuabLuaObject &v);
        void add(const char *key, const QuabLuaObject &v);

        unsigned getLength();
        QuabLuaKeyString* getKeys(unsigned *cnt);

	};	
	
	class QUAB_API QuabLua
	{
	private:
		lua_State *L;
        QuabLua(const QuabLua &lua){}
        QuabLua(){ this->_autoCounter = 0; }
		unsigned _autoCounter;
	public:
		static QuabLua* create(const char *vmName);
        static void release(const char *vmName);
        ~QuabLua();
            
		void set(const char *name, int value);
		void set(const char *name, double value);
		void set(const char *name, const char *str);
		void set(const char *name, bool value);
		void set(const char *name, const QuabLuaTable &tb);
		
		bool exists(const char *name);
            		
        double getNumber(const char *name);
        const char* getString(const char *name);
        bool getBoolean(const char *name);
        QuabLuaTable* getTable(const char *name);
	
		bool exec(const QuabStream *buffer, bool isString = true);
		bool exec(const char *file);
		
		bool registerTo(QuabLuaCallback f, const char *name);
		
		void call(const char *fname, const QuabLuaTable &para);
				
	};
}

#endif
