#include "basescript.h"
#include <lualib.h>
#include <lauxlib.h>

#define MAX_POOL 256

struct qbase_sta	{
	lua_State* L;
	int status;
};

struct	state_pool	{
	qbase_sta pool[MAX_POOL];
	char used_cnt;
	char last;
} _sta_pool;

// static function


// implement the interface
int qbase_lua_init(qbase_sta** sta)	{
	
}

void qbase_lua_close(qbase_sta* sta)	{
	
}

void qbase_lua_exec(char* text, const char* chunk_name, int paramcnt, int retcnt, qbase_ret* ret, qbase_sta* sta)	{
	
}

void qbase_lua_load(char* file, const char* chunk_name, qbase_sta* sta)	{
	
}

void qbase_lua_reg(qbase_regfunc f, qbase_sta* sta)	{
	
}

qbase_ret qbase_lua_call(const char* func_name, qbase_sta* sta)	{
	
}

qbase_ret qbase_lua_get(const char* name, qbase_sta* sta)	{
	
}

qbase_ret qbase_lua_getfield(const char* tbname, const char* fieldname, qbase_sta* sta)	{
	
}
