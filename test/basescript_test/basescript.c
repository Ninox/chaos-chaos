#include "basescript.h"

#ifdef	__cplusplus
extern "C"	{
#endif
#include <lualib.h>
#include <lauxlib.h>
#ifdef	__cplusplus
}
#endif
#define MAX_POOL 256

struct qbase_sta	{
	lua_State* L;
	int status;
};

struct	state_pool	{
	qbase_sta pool[MAX_POOL];
	char used_cnt;
	char last;
};

static state_pool * _sta_pool;

// static function
static int
sta_pool_push(state_pool *_pool, qbase_sta** sta_ptr)	{
	for(int i = 0; i < MAX_POOL; i++)	{
		if(_pool->pool[i].status == 0)	{			
			_pool->pool[i].L = luaL_newstate();
			luaL_openlibs(_pool->pool[i].L);
			_pool->pool[i].status = 1;
			(*sta_ptr) = &_pool->pool[i];
			_pool->last = i;
			return i;
		}
	}
	return -1;
}

// implement the interface
int qbase_lua_init(qbase_sta** sta_ptr)	{
	if(_sta_pool == NULL)
		_sta_pool = (state_pool*)malloc(sizeof(state_pool));
	if(_sta_pool->used_cnt >= MAX_POOL)
		return 1;
	if(sta_pool_push(_sta_pool, sta_ptr))
		return 1;
	else return 0;
}

void qbase_lua_close(qbase_sta* sta)	{
	
}

void qbase_lua_exec(char* text, const char* chunk_name, int paramcnt, int retcnt, qbase_ret* ret, qbase_sta* sta)	{
	
}

void qbase_lua_load(char* file, const char* chunk_name, qbase_sta* sta)	{
	
}

void qbase_lua_reg(qbase_regfunc f, qbase_sta* sta)	{
	
}

qbase_ret qbase_lua_call(const char* func_name, const qbase_ret* params, size_t paramcnt, qbase_sta* sta)	{
	
}

qbase_ret qbase_lua_get(const char* name, qbase_sta* sta)	{
	
}

qbase_ret qbase_lua_getfield(const char* tbname, const char* fieldname, qbase_sta* sta)	{
	
}
