#ifndef QUAB_LUAVALUE_H
#define QUAB_LUAVALUE_H

#include <string>
#include <stddef.h>

namespace Quab
{
	class QuabLuaTable;
	typedef QuabLuaTable* QuabLuaTablePtr;
	
	enum LuaValueType	{
                  VALUE_UNKNOWN    = -2,
			VALUE_NIL        = -1,
			VALUE_NUMBER     =  0,
			VALUE_STRING     =  1,
			VALUE_BOOLEAN    =  2,
			VALUE_TABLE      =  3
	};
	
	class QuabLuaValue
	{		
	private:
		union	{
			double number;
			bool boolean;
		} _value;
            std::string _str;
		QuabLuaTable *_table;
						
	public:
            QuabLuaValue() { this->_table = NULL;}
            QuabLuaValue(double v):_table(NULL) { this->LuaValueType = VALUE_NUMBER; _value.number = v; }
            QuabLuaValue(const char *v):_table(NULL), _str("") { this->LuaValueType = VALUE_STRING; _str.append(v); }
            QuabLuaValue(bool v):_table(NULL) { this->LuaValueType = VALUE_BOOLEAN; _value.boolean = v; }
            QuabLuaValue(QuabLuaTable *v) { this->LuaValueType = VALUE_TABLE; this->_table = v; }

            QuabLuaValue(const QuabLuaValue &v) {
                  this->_value = v._value;
                  this->_table = v._table;
                  this->_str = v._str;
            }

		int LuaValueType;		
		template<typename T> T get();
	};

    template<> 
    int QuabLuaValue::get() { 
        return (int)this->_value.number; 
    }
	template<>
    double QuabLuaValue::get() {
        return this->_value.number; 
    }
	template<> 
    const char * QuabLuaValue::get() {
        return this->_str.c_str(); 
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
