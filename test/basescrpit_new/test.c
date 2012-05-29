#include <stdio.h>
#include <stdlib.h>
#include "basescript.h"

int main()
{
    int i;
    qbase_value *v = NULL;
    qbase_value *v2 = NULL;
    qbase_table *table = NULL;
    qbase_sta *sta = NULL;
    printf("READY?\n");
    getchar();
    qbase_lua_create(&sta);
    qbase_lua_load(sta, "test.lua", 0, FROM_FILE);
    for(i=0; i<100000; i++) {
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
    }
    qbase_lua_free(sta);
    getchar();
    return 0;
}
