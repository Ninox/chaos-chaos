#ifndef QUAB_LUA_H
#define QUAB_LUA_H

#include <string>
#include <vector>
#include <map>

struct lua_State;

namespace Quab
{
	class QuabStream;
    class QuabLuaTable;
    typedef int (*QuabLuaCallback)(lua_State *L);
    typedef const char* QuabLuaKeyString;
	
    struct QuabLuaObject 
    {
        union   {
            double number;
            bool boolean;
            std::string *str;
            QuabLuaTable *table;
        } _value;

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
        QuabLuaObject& operator[](unsigned idx);
        QuabLuaObject& operator[](const char *name);
        unsigned getLength();
        QuabLuaKeyString* getKeys(unsigned *cnt);
	};	
	
	class QUAB_API QuabLua
	{
	private:
		lua_State *L;
        QuabLua(const QuabLua &lua){}
        QuabLua(){}
	public:
        static QuabLua* create(const char *vmName);
        static void release(const char *vmName);
        ~QuabLua();

		void set(const char *name, int value);
		void set(const char *name, double value);
		void set(const char *name, const char *str);
		void set(const char *name, bool value);
		void set(const char *name, const QuabLuaTable &tb);
		
		bool exists(const char *name, int *type);
		
        bool get(const char *name, double *value);
        bool get(const char *name, std::string *str);
        bool get(const char *name, bool *value);
        bool get(const char *name, QuabLuaTable *table);
	
		bool exec(const QuabStream *buffer, bool isString = true);
		bool exec(const char *file);
		
		bool registerTo(QuabLuaCallback f, const char *name);
		
		void call(const char *fname, const QuabLuaTable &params);
				
	};
}

#endif
