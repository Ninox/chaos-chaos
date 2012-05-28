#include "basescript.h"
#include <string.h>
#include <lauxlib.h>
#include <lualib.h>

#define MAX_POOL_SIZE  64

enum pool_status	{
	STA_FREE = 0,
	STA_USED = 1
}

typedef struct	{
	lua_State *L;
	int status;
} qbase_sta;

typedef struct {
	qbase_sta pool[MAX_POOL_SIZE];
	int last;
	int used;
	short has_init;
} state_pool;

static sta_pool _stapool;

/**
*	static helper functions
*/
static void
pool_initialize()	{
	int i;
	_stapool.last = -1;
	_stapool.used = 0;
	_stapool.has_init = 1;
	for(i = 0; i < MAX_POOL_SIZE; i++)	{
		pool[i].L = NULL;
		pool[i].statue = STA_FREE;
	}
}

static qbase_sta *
sta_create()	{
	int idx = (_stapool.last+1) % MAX_POOL_SIZE;
	if(_stapool.used == MAX_POOL_SIZE)
		return NULL;
	while(_stapool.pool[idx].status != STA_FREE)
		idx++;
	_stapool.last = idx;
	_stapool.pool[idx].L = luaL_newstate();
	_stapool.pool[idx].status = STA_USED;
	_stapool.used+=1;
	return &_stapool.pool[idx];
}

static void
sta_release(qbase_sta *sta)	{
	int cur = sta - &_stapool.pool[0]
	lua_close(sta->L);
	sta->status = STA_FREE;
	_stapool.used -= 1;	
	_stapool.last = cur;
}

static qbase_table *
table_create(qbase_table *tb, int idx)	{
	
}

static void
table_drop(qbase_table *tb)	{
}

static int
table_contain(qbase_table *tb, const char *field)	{
	int i;
	if(tb == NULL)
		return 0;
	if(tb->field_count <= 1)
		return 0;
	for(i = 1; i <= tb->field_count; i++)	{
		if(strcmp(tb->fieldnames[i], field) == 0)
			return 1;
	}
	return 0;
}

static qbase_value *
get_luavalue(qbase_sta * sta, int sidx)	{
	qbase_value *v = (qbase_value*)malloc(sizeof(qbase_value));
	const char *t_str = NULL;
	int stackcnt = 0;
	switch(lua_type(sta->L, sidx))
	{
	case LUA_TNIL:
		v->vtype = QB_NIL;
		v->values = NULL;
		break;
	case LUA_TNUMBER:
		v->vtype = QB_NUMBER;
		v->values.nval = lua_tonumber(sta->L, sidx);
		break;
	case LUA_TSTRING:
		v->vtype = QB_STRING;
		t_str = lua_tostring(sta->L, sidx);
		v->values.sval.str = (char*)malloc(strlen(t_str)+1);
		v->values.sval.len = strlen(t_str);
		memcpy(v->values.sval.str, t_str, v->values.sval.len);
		break;
	case LUA_TBOOLEAN:
		v->vtype = QB_BOOLEAN;
		v->values.nval = lua_toboolean(sta->L, sidx);
		break;
	case LUA_TTABLE:
		v->vtype = QB_TABLE;
		v->values.table = table_create(sta->L, sidx);
		break;
	default:
		v->vtype = QB_NIL;
		luaL_error(sta->L, "not support this return type");
		break;
	}
	stackcnt = lua_gettop(sta->L);
	lua_pop(sta->L,stackcnt);
	return v;
}

/**
*	qbase lua implements
**/
int 
qbase_lua_create(const qbase_sta **sta_ptr)	{
	if(_stapool.has_init != 1)
		pool_initialize();
	*(sta_ptr) = sta_create();
	return sta_ptr;
}

int 
qbase_lua_free(qbase_sta *sta)	{
	sta_release(sta);
}

qbase_value* 
qbase_lua_get(qbase_sta *sta, const char *name)	{
	qbase_value *v = NULL;
	lua_getglobal(sta->L, name);	
	v = get_luavalue(sta, -1);
	return v;
}

void 
qbase_lua_freetable(qbase_table *tbl)	{
	table_drop(tbl);
	free(tbl);
}

qbase_value* 
qbase_lua_getfield(qbase_sta *sta, qbase_table *tb, const char *field)	{
	
}

void 
qbase_lua_reg(qbase_sta *sta, const char *name, qbase_regfunc f)	{
	lua_register(sta->L, name, f);
}

qbase_value* 
qbase_lua_load(qbase_sta *sta, const char *buf, size_t retcnt, qbase_buftype from)	{

}

qbase_value* 
qbase_lua_call(qbase_sta *sta, const char *fname, int paramcnt, qbase_value *v, int retcnt)	{
}

qbase_value* 
qbase_lua_exec(qbase_sta *sta, const char *fname, int retcnt)	{
}
