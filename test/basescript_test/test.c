#include <stdio.h>
#include "basescript.h"

int main()
{
	struct qbase_sta * sta = NULL;
	if(qbase_lua_init(&sta) == 0)	
	{
		// todo sth
		qbase_ret ret;
		qbase_lua_exec("print(\"Hello world\\n\")", "test_print", 0, &ret, sta);
		qbase_lua_exec("return 123", "return_test", 1, &ret, sta);
		
		qbase_lua_load("test.lua", "file_test", sta);
		// qbase_lua_call
		ret = qbase_lua_get("bval", sta);
		ret = qbase_lua_get("nval", sta);
		ret = qbase_lua_get("sval", sta);
		ret = qbase_lua_get("tval", sta);
		
		ret = qbase_lua_getfield("smalltb","name",sta);
		ret = qbase_lua_getfield("smalltb","age",sta);
		
		// close the state
		qbase_lua_close(sta);
	}
	else printf("some error happends when initalize the state\n");
	
	system("pause");
	return 0;
}