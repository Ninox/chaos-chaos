#include <stdio.h>
#include <stdlib.h>
#include <Quab/qbase/qbase.h>
#include <xaloy.h>
using namespace xaloy;

XALOY_TEST_MODULE(luaExecute)
{
    qbase_value *v = NULL;
    qbase_value *v2 = NULL;
    qbase_value *v3 = NULL;
    qbase_rets  rets = NULL;
    qbase_table *table = NULL, *tb2 = NULL;
    qbase_sta *sta = NULL;
    qbase_lua_create(&sta);
	// assert to test the sta is not null
	XALOY_ASSERT_NULL(XL_NOTNULL, sta);
	
    qbase_lua_load(sta, "test.lua", 0, FROM_FILE);
    /*      global variable getter      */
    v = qbase_lua_get(sta, "NVal");
	XALOY_ASSERT(XL_NOT_EQUAL, v->vtype, QBS_NIL);
	XALOY_EXPECT(XL_EQUAL, v->values.nval, 9876.0);
    free(v);
	
	v = qbase_lua_get(sta, "NVal2");
	XALOY_ASSERT(XL_EQUAL, v->vtype, QBS_NIL);
	
    v = qbase_lua_get(sta, "BVal");
    XALOY_ASSERT(XL_NOT_EQUAL, v->vtype, QBS_NIL);
	XALOY_EXPECT(XL_EQUAL, v->values.bval, 1);
	printf("v->values:%d\n", v->values.bval);
    free(v);
	
    v = qbase_lua_get(sta, "SVal");
    XALOY_ASSERT(XL_NOT_EQUAL, v->vtype, QBS_NIL);
	XALOY_EXPECT_STR(XL_EQUAL, v->values.sval.str, "Hello world");
    free(v->values.sval.str);
    free(v);

    /*      global table getter      */
    v = qbase_lua_get(sta,"TVal");
	XALOY_ASSERT(XL_NOT_EQUAL, v->vtype, QBS_NIL);	
    table = v->values.table;	
	XALOY_ASSERT_NULL(XL_NOTNULL, table);
	
    v2 = qbase_lua_getfield(sta, table, "num");
	XALOY_ASSERT(XL_NOT_EQUAL, v2->vtype, QBS_NIL);
	XALOY_EXPECT(XL_EQUAL, v2->values.nval, 886.0);
    free(v2);
	
    v2 = qbase_lua_getfield(sta, table, "str");
	XALOY_ASSERT(XL_NOT_EQUAL, v2->vtype, QBS_NIL);
	XALOY_EXPECT_STR(XL_EQUAL, v2->values.sval.str, "table");
    free(v2->values.sval.str);
    free(v2);
	
    v2 = qbase_lua_getfield(sta, table, "bol");
	XALOY_ASSERT(XL_NOT_EQUAL, v2->vtype, QBS_NIL);
	XALOY_EXPECT(XL_EQUAL, v2->values.bval, 1);
    free(v2);
	
    qbase_lua_freetable(&table);
    free(v);

    /*      inner table getter      */
    v = qbase_lua_get(sta,"TVal");
	XALOY_ASSERT(XL_NOT_EQUAL, v->vtype, QBS_NIL);
    table = v->values.table;	
	XALOY_ASSERT(XL_EQUAL, v->vtype, QBS_TABLE);
	
    v2 = qbase_lua_getfield(sta, table, "tab");
	XALOY_ASSERT(XL_NOT_EQUAL, v2->vtype, QBS_NIL);
    tb2 = v2->values.table;
	XALOY_ASSERT(XL_EQUAL, v2->vtype, QBS_TABLE);
	
    v3 = qbase_lua_getfield(sta, tb2, "n");
	XALOY_ASSERT(XL_NOT_EQUAL, v3->vtype, QBS_NIL);
	XALOY_EXPECT(XL_EQUAL, v3->values.nval, 1.0);
    free(v3);
	
    v3 = qbase_lua_getfield(sta, tb2, "s");
	XALOY_ASSERT(XL_NOT_EQUAL, v3->vtype, QBS_NIL);
	XALOY_EXPECT_STR(XL_EQUAL, v3->values.sval.str, "string");
    free(v3->values.sval.str);
    free(v3);
	
    qbase_lua_freetable(&tb2);
    free(v2);
    qbase_lua_freetable(&table);
    free(v);

    /*      function executers      */
    rets = qbase_lua_exec(sta, "local a=1;local b=2; print(a+b);",0);
	XALOY_ASSERT_NULL(XL_ISNULL, rets);
    if(rets != NULL)
        free(rets);
    rets = qbase_lua_call(sta, "FVal", 0, NULL, 0);
	XALOY_ASSERT_NULL(XL_ISNULL, rets);
    if(rets != NULL)
        free(rets);
    qbase_lua_free(&sta);
}


int main()
{    
    return G_XALOY.execute();
}
