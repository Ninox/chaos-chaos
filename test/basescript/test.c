#include <stdlib.h>
#include <stdio.h>
#include "basescript.h"

static int
sayhello(lua_State *L)   {
    int a = lua_tointeger(L, -1);
    int b = lua_tointeger(L, -2);
    lua_pushnumber(L, a*b);
    return 1;
}

int main()
{
	struct qbase_sta * sta = NULL;
	if(qbase_lua_init(&sta) == 0)
	{
// 	    todo sth
		qbase_ret ret;
		printf("%d\n",sizeof(ret));
		qbase_lua_exec("print(\"Hello world\\n\")", "test_print", 0, 0,sta);
		qbase_lua_exec("sum=0;for i=1,100 do sum=sum+i; end print(sum);", "return_test", 0,0, sta);

		qbase_lua_load("test.lua", "file_test", 0, 0, sta);
//		qbase_lua_call
		ret = qbase_lua_get("bval", sta);
		printf("ret = %s\n",ret.val.bool_val!=0?"true":"false");
		ret = qbase_lua_get("nval", sta);
		printf("ret = %f\n",ret.val.number_val);
		ret = qbase_lua_get("sval", sta);
		printf("ret = %s\n",ret.val.str_val.str);
		ret = qbase_lua_get("tval", sta);
//		printf("ret = %d\n",ret.val.number_val);

		ret = qbase_lua_getfield("smalltb","name",sta);
		printf("%s['%s'] => %s\n","smalltb","name",ret.val.str_val.str);
		ret = qbase_lua_getfield("smalltb","age",sta);
		printf("%s['%s'] => %f\n","smalltb","age",ret.val.number_val);

        qbase_lua_reg(sayhello, "sayfunc", sta);
        qbase_lua_exec("print(\"FUCK U!!!\"..sayfunc(1,2))","superfunc", 0, NULL, sta);
        qbase_lua_call("goodtest",NULL,0,0,sta);
//		close the state
		qbase_lua_close(sta);
	}
	else printf("some error happends when initalize the state\n");
	system("pause");
	return 0;
}
