#ifndef QUAB_LUAVALUE_H
#define QUAB_LUAVALUE_H

namespace Quab
{

	class QuabLuaTable;
	typedef QuabLuaTable* QuabLuaTablePtr;
	typedef const char* LuaString;
	
	class QuabValue
	{
		enum ValueType	{
			VALUE_INT     = 0,
			VALUE_STRING  = 1,
			VALUE_DOUBLE  = 2,
			VALUE_BOOLEAN = 3,
			VALUE_TABLE   = 4
		};
	private:
		union	{
			int integer;
			double doublef;
			LuaString str;
			bool boolean;
		} _value;
		
		QuabLuaTable *_table;
						
	public:
		public QuabValue(int v): { this->LuaValueType = VALUE_INT; _value.integer = v; }
		public QuabValue(double v) { this->LuaValueType = VALUE_DOUBLE; _value.doublef = v; }
		public QuabValue(const char *v) { this->LuaValueType = VALUE_STRING; _value.str = v; }
		public QuabValue(bool v) { this->LuaValueType = VALUE_BOOLEAN; _value.boolean = v; }
		public QuabValue(QuabLuaTable *v) { this->LuaValueType = VALUE_TABLE; this->_value = v; }
	
		int LuaValueType;
		
		template<typename T> T get();		
		template<> int get() { return this->_value.integer; }
		template<> double get() { return this->_value.doublef; }
		template<> LuaString get() { return this->_value.str; }
		template<> bool get() { return this->_value.boolean; }
		template<> QuabLuaTablePtr get() { return this->_table; }
	};
}

#endif // QUAB_LUAVALUE_H