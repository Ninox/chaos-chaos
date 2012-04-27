#ifndef QBASE_LUA_H
#define QBASE_LUA_H

#include <stddef.h>

#ifdef	__cplusplus
extern "C"	{
#endif
#include <lua.h>
#ifdef	__cplusplus
}
#endif

typedef struct	{
	const char* str;
	size_t len;
} qbase_str

typedef struct	{
	const char** data;
	size_t size;
} qbase_tbl;

typedef struct	{
	int ret_type;
	union	{
		double number_val;
		int bool_val;	
		qbase_str str_val;
		qbase_tbl* tbl_val;
	} val;
	enum	{
		NUMBER = 0,
		BOOLEAN=  1,
		STRING = 2,
		TABLE = 3
	}
} qbase_ret;

struct qbase_sta;

int qbase_lua_init(qbase_sta** sta);
void qbase_lua_close(qbase_sta* sta);
void qbase_lua_exec(char* text, const char* chunk_name, int retcnt, qbase_ret* ret, qbase_sta* sta);
void qbase_lua_load(char* file, const char* chunk_name, int retcnt, qbase_ret* ret, qbase_sta* sta);
void qbase_lua_reg(qbase_regfunc f, qbase_sta* sta);
qbase_ret* qbase_lua_call(const char* func_name, const qbase_ret* params, size_t paramcnt, size_t retcnt, qbase_sta* sta);
qbase_ret qbase_lua_get(const char* name, qbase_sta* sta);
qbase_ret qbase_lua_getfield(const char* tbname, const char* fieldname, qbase_sta* sta);

#endif