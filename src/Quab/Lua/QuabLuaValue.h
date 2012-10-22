#ifndef QUAB_LUAVALUE_H
#define QUAB_LUAVALUE_H

namespace Quab
{

	class QuabLuaTable;
	typedef QuabLuaTable* QuabLuaTablePtr;
	typedef const char* LuaString;
	
	enum LuaValueType	{
			VALUE_INT     = 0,
			VALUE_STRING  = 1,
			VALUE_DOUBLE  = 2,
			VALUE_BOOLEAN = 3,
			VALUE_TABLE   = 4
	};
	
	class QuabLuaValue
	{		
	private:
		union	{
			int integer;
			double doublef;
			LuaString str;
			bool boolean;
		} _value;
		
		QuabLuaTable *_table;
						
	public:
            QuabLuaValue() { this->_table = NULL;}
		QuabLuaValue(int v):_table(NULL) { this->LuaValueType = VALUE_INT; _value.integer = v; }
		QuabLuaValue(double v):_table(NULL) { this->LuaValueType = VALUE_DOUBLE; _value.doublef = v; }
		QuabLuaValue(const char *v):_table(NULL) { this->LuaValueType = VALUE_STRING; _value.str = v; }
		QuabLuaValue(bool v):_table(NULL) { this->LuaValueType = VALUE_BOOLEAN; _value.boolean = v; }
		QuabLuaValue(QuabLuaTable *v):_table(NULL) { this->LuaValueType = VALUE_TABLE; this->_table = v; }
	
		int LuaValueType;
		
		template<typename T> T get();
	};

    template<> 
    int QuabLuaValue::get() { 
        return this->_value.integer; 
    }
	template<>
    double QuabLuaValue::get() {
        return this->_value.doublef; 
    }
	template<> 
    LuaString QuabLuaValue::get() {
        return this->_value.str; 
    }
	template<> 
   bool QuabLuaValue::get() {
       return this->_value.boolean; 
   }
	template<> 
    QuabLuaTablePtr QuabLuaValue::get() {
        return this->_table; 
    }
}

#endif // QUAB_LUAVALUE_H
