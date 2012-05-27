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

static void
set_luaret(qbase_sta *sta, qbase_ret *ret, int idx)  {
    char *tbname = NULL, *buffstr = NULL;
    char tNum[20];
    const char *tempStr = NULL;

    if(ret == NULL)
        return;
    switch(lua_type(sta->L, idx))
	{
	case LUA_TNIL:
		(*ret).ret_type = NIL;
		break;
	case LUA_TNUMBER:
		if(lua_isnumber(sta->L, idx))	{
			(*ret).ret_type = NUMBER;
			(*ret).val.number_val = lua_tonumber(sta->L, idx);
		}
		else (*ret).ret_type = NIL;
		break;
	case LUA_TBOOLEAN:
		if(lua_isboolean(sta->L,idx))	{
			(*ret).ret_type = BOOLEAN;
			(*ret).val.bool_val = lua_toboolean(sta->L, idx);
		}
		else (*ret).ret_type = NIL;
		break;
	case LUA_TSTRING:
		if(lua_isstring(sta->L, idx))	{
			(*ret).ret_type = STRING;
			tempStr = lua_tostring(sta->L, idx);
			(*ret).val.str_val.len = strlen(tempStr);
			buffstr = (char*)malloc(sizeof(char)* (*ret).val.str_val.len+1);
			strcpy(buffstr, tempStr);
			(*ret).val.str_val.str = buffstr;
		}
		else (*ret).ret_type = NIL;
		break;
	case LUA_TTABLE:
		// save a table as a global varible in lua, the return is a temp name for getting it;
		// TODO: next version will implement this with a real table structure.
		if(lua_istable(sta->L, idx))	{
			(*ret).ret_type = TABLE;
			tbname = (char*)malloc(sizeof(char)*20);
			memset(tbname, 0, 20);
			strcpy(tbname, "table_");
			sprintf(tNum, "%u", (unsigned)time(0));
			strcat(tbname, tNum);
			(*ret).val.str_val.str = tbname;
			(*ret).val.str_val.len = strlen(tbname);
			lua_setglobal(sta->L, tbname);
		}
		else (*ret).ret_type = NIL;
		break;
	default:
		(*ret).ret_type = NIL;
		luaL_error(sta->L, "not support");
		break;
	}
}

// implement the interface
int qbase_lua_init(qbase_sta** sta_ptr)	{
    sta_pool_init();
	if(_sta_pool->used_cnt >= MAX_POOL)
		return 1;
	if(sta_pool_push(_sta_pool, sta_ptr) < 0)
		return 1;
	else    {
	    // update the pool count
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

int qbase_lua_exec(char* text,int retcnt, qbase_ret* ret, qbase_sta* sta)	{
	// set the parameters before load the buffer
	int stack_count = 0;
	int i;
	if(!luaL_dostring(sta->L, text))	{
        // get returns
        if(ret != NULL && retcnt > 0)   {
            for(i = 0; i < retcnt; i++) {
                set_luaret(sta, &ret[i], -1-i);
            }
        }
        stack_count = lua_gettop(sta->L);
        lua_pop(sta->L, stack_count);
		return 1;
	}
	else return 0;
}

int qbase_lua_load(char* file, int retcnt, qbase_ret* ret, qbase_sta* sta)	{
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
	qbase_ret *rets = NULL;
	int i, topcount = 0;
	lua_getglobal(sta->L, func_name);
	// check the funcion is exists
	if(lua_type(sta->L, -1) == LUA_TNIL)    {
        topcount = lua_gettop(sta->L);
        lua_pop(sta->L, topcount);
        return NULL;
	}
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
	if(!lua_pcall(sta->L, paramcnt, retcnt, 0)) {
        if(retcnt > 0)  {
            rets = (qbase_ret*)malloc(sizeof(qbase_ret)*retcnt);
            for(i = 0; i < retcnt; i++) {
                set_luaret(sta, &rets[i], -1-i);
            }
        }
	}
	// pop it
	topcount = lua_gettop(sta->L);
	lua_pop(sta->L, topcount);
	return rets;

}

// TODO: refactor the get & getfield, use a common getter instead of the switch part in function
struct qbase_ret qbase_lua_get(const char* name, qbase_sta* sta)	{
	// lua return
	struct qbase_ret ret;
	int stackcnt =0;

	lua_getglobal(sta->L, name);
	set_luaret(sta, &ret, -1);
	stackcnt = lua_gettop(sta->L);
    lua_pop(sta->L,stackcnt);
	return ret;
}

struct qbase_ret qbase_lua_getfield(const char* tbname, const char* fieldname, qbase_sta* sta)	{
	// lua return
	struct qbase_ret ret;
	int stackcnt = 0;

	lua_getglobal(sta->L, tbname);
	lua_pushstring(sta->L, fieldname);
	lua_gettable(sta->L, -2);
	set_luaret(sta, &ret, -1);
	stackcnt = lua_gettop(sta->L);
    lua_pop(sta->L,stackcnt);
	return ret;
}
