#ifndef QBASE_LUA_H
#define QBASE_LUA_H
#include <stddef.h>
#include <lua.h>

enum qbase_vtypes	{
	QBS_NIL 	= 0,
	QBS_NUMBER 	= 1,
	QBS_BOOLEAN	= 2,
	QBS_STRING 	= 3,
	QBS_TABLE 	= 4,
	QBS_UNKNOW 	= 5
};
enum qbase_buftype	{
	FROM_BUFFER = 0,
	FROM_FILE = 1
};

typedef struct qbase_string	{
	char *str;
	size_t len;
} qbase_bytes;
typedef struct qbase_table	{
	// the fieldnames[0] means table name
	char **fieldnames;
	size_t field_count;
} qbase_table;
typedef struct qbase_value		{
	int vtype;
	union	{
		int	bval;
		double nval;
		qbase_string sval;		
	} values;
};
typedef struct qbase_sta qbase_sta;
typedef lua_CFunction qbase_regfunc;

/**
*	qbase lua initalization functions
**/
int qbase_lua_create(const qbase_sta **sta);
int qbase_lua_free(qbase_sta *sta);

/**
*	get lua's variable
**/
qbase_value* qbase_lua_get(qbase_sta *sta, const char *name);
void qbase_lua_freetable(qbase_table *tbl);
qbase_value* qbase_lua_getfield(qbase_sta *sta, qbase_table *tb, const char *field);

/**
*	function executer
**/
void qbase_lua_reg(qbase_sta *sta, const char *name, qbase_regfunc f);
qbase_value* qbase_lua_load(qbase_sta *sta, const char *buf, size_t retcnt, qbase_buftype from);
qbase_value* qbase_lua_call(qbase_sta *sta, const char *fname, int paramcnt, qbase_value *v, int retcnt);
qbase_value* qbase_lua_exec(qbase_sta *sta, const char *fname, int retcnt);

#endif
