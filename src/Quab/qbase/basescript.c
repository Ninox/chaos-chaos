#include "basescript.h"
#include <lauxlib.h>
#include <lualib.h>

typedef struct	{
	lua_State *L;
	int status;
} qbase_sta;

/**
*	helper functions
*/


/**
*	qbase lua implements
**/
int qbase_lua_create(const qbase_sta **sta);
int qbase_lua_free(qbase_sta *sta);

qbase_value* qbase_lua_get(qbase_sta *sta, const char *name);
void qbase_lua_freetable(qbase_table *tbl);
qbase_value* qbase_lua_getfield(qbase_sta *sta, qbase_table *tb, const char *field);

void qbase_lua_reg(qbase_sta *sta, const char *name, qbase_regfunc f);
qbase_value* qbase_lua_load(qbase_sta *sta, const char *buf, size_t retcnt, qbase_buftype from);
qbase_value* qbase_lua_call(qbase_sta *sta, const char *fname, int paramcnt, qbase_value *v, int retcnt);
qbase_value* qbase_lua_exec(qbase_sta *sta, const char *fname, int retcnt);