#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "basescript.h"

static int
sayhello(lua_State *L)   {
    int a = lua_tointeger(L, -1);
    int b = lua_tointeger(L, -2);
    lua_pushnumber(L, a*2+b);
    return 1;
}

static int
test_normal()   {
    qbase_sta *sta = NULL;
    qbase_lua_init(&sta);
    qbase_lua_load("test.lua", 0, NULL,sta);
    qbase_lua_reg(sayhello, "sayfunc", sta);
    qbase_lua_exec("print(\"FUCK U!!!\"..sayfunc(1,2))", 0, NULL, sta);
    qbase_lua_call("goodtest",NULL,0,0,sta);

    // input the parameter for call
    qbase_ret params[2];
    params[0].ret_type = NUMBER;
    params[0].val.number_val = 1;
    params[1].ret_type = NUMBER;
    params[1].val.number_val = 2;
    qbase_lua_call("bestprint", &params[0], 2, 1, sta);

    qbase_lua_close(sta);
	return 1;
}

static int
test_loop() {
    qbase_sta *sta = NULL;
    int i;
    qbase_lua_init(&sta);
    qbase_lua_load("test.lua", 0, NULL,sta);
    printf("====================================   loop    ===================================\n");
    for(i = 0; i < 10000; i++)   {
        qbase_lua_reg(sayhello, "sayfunc", sta);
        qbase_lua_exec("print(\"FUCK U!!!\"..sayfunc(1,2))", 0, NULL, sta);
        qbase_lua_call("goodtest",NULL,0,0,sta);

        // input the parameter for call
        qbase_ret params[2];
        params[0].ret_type = NUMBER;
        params[0].val.number_val = 1;
        params[1].ret_type = NUMBER;
        params[1].val.number_val = 2;
        qbase_lua_call("bestprint", &params[0], 2, 1, sta);
    }
    qbase_lua_close(sta);
	return 1;
}

int main()
{
    assert(test_normal()==1);
    assert(test_loop()==1);
	return 0;
}

