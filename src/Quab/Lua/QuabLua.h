#ifndef QUAB_LUA_H
#define QUAB_LUA_H

#include <vector>
#include <map>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "../QuabDef.h"

#define QUAB_LUA_AUTOPOP 23

struct lua_State;

namespace Quab
{
    class QuabStream;
    class QuabLuaTable;
    class QuabLuaString;
    typedef int (*QuabLuaCallback)(lua_State *L);
    typedef const char* QuabLuaKeyString; 
	
    struct QuabLuaObject 
    {
        union   {
            double number;
            bool boolean;
            QuabLuaString *str;
            QuabLuaTable *table;
        } _value;

        int type;
    };

    class QuabLuaString
    {
    private:
        char *buffer;
        unsigned _sz;
        QuabLuaString(const QuabLuaString &qls){}
    public:
        QuabLuaString() { this->buffer = NULL; this->_sz = 0; }
        QuabLuaString(const char *str) { this->buffer = NULL; this->setstr(str); }
        ~QuabLuaString() { 
            if(this->buffer != NULL)
                free(this->buffer);
            this->buffer = NULL;
            this->_sz = 0;
        }
        void setstr(const char *s) {
            unsigned len = strlen(s);
            if(this->buffer != NULL)
                free(this->buffer);
            this->buffer = (char*)malloc(len + 1);
            strcpy(this->buffer, s);
            this->_sz = len;
        }
        unsigned getLength() { return this->_sz; }
        const char *str() { return this->buffer; }
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
		
		bool exists(const char *name, int *type);
            		
        void get(const char *name, double *value);
        void get(const char *name, QuabLuaString *str);
        void get(const char *name, bool *value);
        void get(const char *name, QuabLuaTable *table);
	
		bool exec(const QuabStream *buffer, bool isString = true);
		bool exec(const char *file);
		
		bool registerTo(QuabLuaCallback f, const char *name);
		
		void call(const char *fname, const QuabLuaTable &para);
				
	};
}

#endif
