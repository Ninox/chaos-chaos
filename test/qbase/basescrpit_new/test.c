#include <stdio.h>
#include <stdlib.h>
#include "basescript.h"

int main()
{
    int i;
    qbase_value *v = NULL;
    qbase_value *v2 = NULL;
    qbase_value *v3 = NULL;
    qbase_rets  rets = NULL;
    qbase_table *table = NULL, *tb2 = NULL;
    qbase_sta *sta = NULL;
    printf("READY?\n");
    getchar();
    qbase_lua_create(&sta);
    qbase_lua_load(sta, "test.lua", 0, FROM_FILE);
    for(i=0; i<10000; i++) {
        /*      global variable getter      */
        v = qbase_lua_get(sta, "NVal");
        printf("NVal = %f\n",v->values.nval);
        free(v);
        v = qbase_lua_get(sta, "BVal");
        printf("BVal = %d\n",v->values.bval);
        free(v);
        v = qbase_lua_get(sta, "SVal");
        printf("SVal = %s\n",v->values.sval.str);
        free(v->values.sval.str);
        free(v);

        /*      global table getter      */
        v = qbase_lua_get(sta,"TVal");
        table = v->values.table;
        v2 = qbase_lua_getfield(sta, table, "num");
        printf("table.num=%f\n", v2->values.nval);
        free(v2);
        v2 = qbase_lua_getfield(sta, table, "str");
        printf("table.str='%s'\n", v2->values.sval.str);
        free(v2->values.sval.str);
        free(v2);
        v2 = qbase_lua_getfield(sta, table, "bol");
        printf("table.bool='%d'\n", v2->values.bval);
        free(v2);
        qbase_lua_freetable(table);
        free(v);

        /*      inner table getter      */
        v = qbase_lua_get(sta,"TVal");
        table = v->values.table;
        v2 = qbase_lua_getfield(sta, table, "tab");
        tb2 = v2->values.table;
        v3 = qbase_lua_getfield(sta, tb2, "n");
        printf("table.tab.n=%f\n", v3->values.nval);
        free(v3);
        v3 = qbase_lua_getfield(sta, tb2, "s");
        printf("table.tab.s=%s\n", v3->values.sval.str);
        free(v3->values.sval.str);
        free(v3);
        qbase_lua_freetable(tb2);
        free(v2);
        qbase_lua_freetable(table);
        free(v);

        /*      function executers      */
        rets = qbase_lua_exec(sta, "local a=1;local b=2; print(a+b);",0);
        if(rets != NULL)
            free(rets);
        rets = qbase_lua_call(sta, "FVal", 0, NULL, 0);
        if(rets != NULL)
            free(rets);
    }
    qbase_lua_free(sta);
    getchar();
    return 0;
}
