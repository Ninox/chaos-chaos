#ifndef QBASE_LUA_H
#define QBASE_LUA_H

#include <stddef.h>
#include <lua.h>

enum qbase_ret_type	{
		NIL	= 0,
		NUMBER = 1,
		BOOLEAN=  2,
		STRING = 3,
		TABLE = 4,
		UNKNOW = 5
};

typedef struct qbase_str	{
	const char* str;
	size_t len;
} qbase_str;

typedef struct qbase_tbl	{
	const char** data;
	size_t size;
} qbase_tbl;

typedef struct qbase_ret	{
	union	{
		double number_val;
		int bool_val;
		qbase_str str_val;
		qbase_tbl* tbl_val;
	} val;
	int ret_type;
} qbase_ret;

typedef void* (*qbase_regfunc)(lua_State* L, void* params, size_t pcnt);

typedef struct qbase_sta qbase_sta;

int qbase_lua_init(qbase_sta** sta);
void qbase_lua_close(struct qbase_sta* sta);
int qbase_lua_exec(char* text, const char* chunk_name, int retcnt, qbase_ret* ret, qbase_sta* sta);
int qbase_lua_load(char* file, const char* chunk_name, int retcnt, qbase_ret* ret, qbase_sta* sta);
void qbase_lua_reg(qbase_regfunc f, qbase_sta* sta);
qbase_ret* qbase_lua_call(const char* func_name, const qbase_ret* params, size_t paramcnt, size_t retcnt, qbase_sta* sta);
qbase_ret qbase_lua_get(const char* name, qbase_sta* sta);
qbase_ret qbase_lua_getfield(const char* tbname, const char* fieldname, qbase_sta* sta);

#endif
