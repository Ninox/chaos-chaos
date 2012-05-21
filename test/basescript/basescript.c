#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "basescript.h"
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define MAX_POOL 16

// qbase_sta status
#define NOT_USE 0
#define USED    1

struct qbase_sta	{
	lua_State* L;
	int status;
};

struct state_pool	{
	struct qbase_sta pool[MAX_POOL];
	unsigned int used_cnt;
	unsigned char last;
};

typedef struct state_pool state_pool;
static state_pool* _sta_pool;

// static function
static void
sta_pool_init() {
    if(_sta_pool != NULL)
        return;
    _sta_pool = (state_pool*)malloc(sizeof(state_pool));
    _sta_pool->last = 0;
    _sta_pool->used_cnt = 0;
    int i;
    for(i = 0; i < MAX_POOL; i++)   {
        _sta_pool->pool[i].L = NULL;
        _sta_pool->pool[i].status = NOT_USE;
    }
}

static int
sta_pool_push(state_pool *_pool, qbase_sta** sta_ptr)	{
    int i;
	for(i = 0; i < MAX_POOL; i++)	{
		if(_pool->pool[i].status == NOT_USE)	{
			_pool->pool[i].L = luaL_newstate();
			luaL_openlibs(_pool->pool[i].L);
			_pool->pool[i].status = USED;
			(*sta_ptr) = &_pool->pool[i];
			_pool->last = i;
			return i;
		}
	}
	return -1;
}

static void
sta_pool_updatecnt(state_pool * _pool)	{
	int cnt = 0, i;
	for(i = 0; i < MAX_POOL; i++)
		if(_pool->pool[i].status == 1) cnt++;
	_pool->used_cnt = cnt;
}

// implement the interface
int qbase_lua_init(qbase_sta** sta_ptr)	{
    sta_pool_init();
	if(_sta_pool->used_cnt >= MAX_POOL)
		return 1;
	if(sta_pool_push(_sta_pool, sta_ptr) < 0)
		return 1;
	else    {
	    // 更新总数
        sta_pool_updatecnt(_sta_pool);
        return 0;
	}
}

void qbase_lua_close(qbase_sta* sta)	{
	lua_gc(sta->L, LUA_GCCOLLECT, 0);
	lua_close(sta->L);
	_sta_pool->used_cnt--;
	sta->status = 0;
	sta_pool_updatecnt(_sta_pool);
}

int qbase_lua_exec(char* text, const char* chunk_name, int retcnt, qbase_ret* ret, qbase_sta* sta)	{
	// set the parameters before load the buffer
	if(!luaL_loadbuffer(sta->L, text, strlen(text),chunk_name))	{
        lua_pcall(sta->L, 0, 0, 0);
		return 2;
	}
	else return 1;
}

int qbase_lua_load(char* file, const char* chunk_name, int retcnt, qbase_ret* ret, qbase_sta* sta)	{
	// set the parameters before load file
	FILE* f = fopen(file,"r");
	if(f == NULL)
		return 1;
	if(!luaL_dofile(sta->L, file))	{
		return 2;
	}
	else return 0;
}

void qbase_lua_reg(qbase_regfunc f, const char *name, qbase_sta* sta)   {
    lua_register(sta->L, name, f);
}

qbase_ret* qbase_lua_call(const char* func_name, const qbase_ret* params, size_t paramcnt, size_t retcnt, qbase_sta* sta)	{
	qbase_ret*	rets = NULL;
	lua_getglobal(sta->L, func_name);
	int i;
	for(i = 0; i < paramcnt; i++)	{
		switch(params[i].ret_type)
		{
		case NUMBER	:
			lua_pushnumber(sta->L, params[i].val.number_val);
			break;
		case BOOLEAN	:
			lua_pushboolean(sta->L, params[i].val.bool_val);
			break;
		case STRING	:
			lua_pushstring(sta->L, params[i].val.str_val.str);
			break;
		default:
			// more datatype will support in next version
			luaL_error(sta->L, "not support");
			break;
		}
	}
	// call functions	&	return values
	lua_pcall(sta->L, paramcnt, retcnt, 0);
	// pop it
	lua_pop(sta->L, retcnt);
	return rets;

}

// TODO: refactor the get & getfield, use a common getter instead of the switch part in function
struct qbase_ret qbase_lua_get(const char* name, qbase_sta* sta)	{
	// lua return
	struct qbase_ret ret;
	char *tbname = NULL;

	lua_getglobal(sta->L, name);
	switch(lua_type(sta->L, -1))
	{
	case LUA_TNIL:
		ret.ret_type = NIL;
		break;
	case LUA_TNUMBER:
		if(lua_isnumber(sta->L, -1))	{
			ret.ret_type = NUMBER;
			ret.val.number_val = lua_tonumber(sta->L, -1);
		}
		else ret.ret_type = NIL;
		break;
	case LUA_TBOOLEAN:
		if(lua_isboolean(sta->L,-1))	{
			ret.ret_type = BOOLEAN;
			ret.val.bool_val = lua_toboolean(sta->L, -1);
		}
		else ret.ret_type = NIL;
		break;
	case LUA_TSTRING:
		if(lua_isstring(sta->L, -1))	{
			ret.ret_type = STRING;
			ret.val.str_val.str = lua_tostring(sta->L, -1);
			ret.val.str_val.len = strlen(ret.val.str_val.str);
		}
		else ret.ret_type = NIL;
		break;
	case LUA_TTABLE:
		// save a table as a global varible in lua, the return is a temp name for getting it;
		// TODO: next version will implement this with a real table structure.
		if(lua_istable(sta->L, -1))	{
			ret.ret_type = TABLE;
			tbname = (char*)malloc(sizeof(char)*20);
			memset(tbname, 0, 20);
			strcpy(tbname, "table_");
			char tNum[12];
			sprintf(tNum, "%u", (unsigned)time(0));
			strcat(tbname, tNum);
			ret.val.str_val.str = tbname;
			ret.val.str_val.len = strlen(tbname);
			lua_setglobal(sta->L, tbname);
		}
		else ret.ret_type = NIL;
		break;
	default:
		ret.ret_type = NIL;
		luaL_error(sta->L, "not support");
		break;
	}
	// it the return is table. do not pop it
	if(tbname == NULL)
		lua_pop(sta->L,1);
	return ret;
}

struct qbase_ret qbase_lua_getfield(const char* tbname, const char* fieldname, qbase_sta* sta)	{
	// lua return
	struct qbase_ret ret;
	char *tablename = NULL;

	lua_getglobal(sta->L, tbname);
	lua_pushstring(sta->L, fieldname);
	lua_gettable(sta->L, -2);
	switch(lua_type(sta->L, -1))
	{
	case LUA_TNIL:
		ret.ret_type = NIL;
		break;
	case LUA_TNUMBER:
		ret.ret_type = NUMBER;
		ret.val.number_val = lua_tonumber(sta->L, -1);
		break;
	case LUA_TBOOLEAN:
		ret.ret_type = BOOLEAN;
		ret.val.bool_val = lua_toboolean(sta->L,-1);
		break;
	case LUA_TSTRING:
		ret.ret_type = STRING;
		ret.val.str_val.str = lua_tostring(sta->L,-1);
		ret.val.str_val.len = strlen(ret.val.str_val.str);
		break;
	case LUA_TTABLE:
		ret.ret_type = TABLE;
		tablename = (char*)malloc(sizeof(char)*20);
		memset(tablename, 0, 20);
		sprintf(tablename, "%u", (unsigned)time(0));
		ret.val.str_val.str = tablename;
		ret.val.str_val.len = strlen(tablename);
		lua_setglobal(sta->L, tablename);
		break;
	default:
		ret.ret_type = NIL;
		luaL_error(sta->L, "not support");
		break;
	}
	if(tbname == NULL)
		lua_pop(sta->L, 1);
	return ret;
}
