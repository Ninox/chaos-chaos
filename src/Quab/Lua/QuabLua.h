#ifndef QUAB_LUA_H
#define QUAB_LUA_H

#include <vector>
#include "QuabLuaValue.h"

struct lua_State;

namespace Quab
{
	class QuabStream;
      typedef int (*QuabLuaCallback)(lua_State *L);
	
	class QuabLuaTable
	{
	private:
	public:
	};	
	
	class QuabLua
	{
	private:
		lua_State *L;
            QuabLua(const QuabLua &ql){}
	public:
            QuabLua();
            ~QuabLua();

		void set(const char *name, int value);
		void set(const char *name, double value);
		void set(const char *name, const char *str);
		void set(const char *name, bool value);
		void set(const char *name, const QuabLuaTable &tb);
		
		bool exists(const char *name, int *type);
		
		QuabLuaValue get(const char *name);	
	
		bool load(const QuabStream *buffer);
		bool load(const char *path);		
		void exec(const QuabStream *buffer);
		void exec(const char *file);
		
		bool registerTo(QuabLuaCallback f, const char *name);
		
		void call(const char *fname, const std::vector<QuabLuaValue> &param);
				
	};
}

#endif
