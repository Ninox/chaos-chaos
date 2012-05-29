#include "basescript.h"
#include <stdlib.h>
#include <string.h>
#include <lauxlib.h>
#include <lualib.h>

#define MAX_POOL_SIZE  64

enum pool_status	{
	STA_FREE = 0,
	STA_USED = 1
};

struct qbase_sta {
	lua_State *L;
	int status;
} ;

struct qbase_table  {
	// the fieldnames[0] means table name
	char **fieldnames;
	size_t field_count;
};

typedef struct {
	qbase_sta pool[MAX_POOL_SIZE];
	int last;
	int used;
	short has_init;
} state_pool;

static state_pool _stapool;

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
		_stapool.pool[i].L = NULL;
		_stapool.pool[i].status = STA_FREE;
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
	luaL_openlibs(_stapool.pool[idx].L);
	_stapool.pool[idx].status = STA_USED;
	_stapool.used+=1;
	return &_stapool.pool[idx];
}

static void
sta_release(qbase_sta *sta)	{
	lua_close(sta->L);
	free(sta->L);
	sta->L = NULL;
	sta->status = STA_FREE;
	_stapool.used -= 1;
}

static int
table_len(qbase_sta *sta)	{
	int len = 0;
	lua_pushnil(sta->L);
	while(0 != lua_next(sta->L, -2))    {
		len+=1;
		lua_pop(sta->L, 1);
	}
	return len;
}

static qbase_table *
table_create(qbase_sta *sta)	{
	int tblen = 0, i = 1;
	if(!lua_istable(sta->L, -1))
		return NULL;
	tblen = table_len(sta);
	const char *keystr = NULL;
	qbase_table *t = (qbase_table*)malloc(sizeof(qbase_table));
	t->fieldnames = (char**)malloc(sizeof(char*)*(tblen+1));
	t->field_count = tblen;
	// TODO: set table name in fieldnames[0]
	lua_pushnil(sta->L);
	while(0 != lua_next(sta->L, -2))	{
		keystr = lua_tostring(sta->L, -2);
		t->fieldnames[i] = (char*)malloc(strlen(keystr)+1);
		strcpy(t->fieldnames[i], keystr);
		lua_pop(sta->L, 1);
		i++;
	}
	return t;
}

static void
table_drop(qbase_table *tb)	{
	int len = tb->field_count+1;
	int i;
	for(i = 0; i < len; i++)	{
		free(tb->fieldnames[i]);
		tb->fieldnames[i] = NULL;
	}
	free(tb->fieldnames);
	tb->fieldnames = NULL;
}

static qbase_value *
get_luavalue(qbase_sta * sta)	{
	qbase_value *v = (qbase_value*)malloc(sizeof(qbase_value));
	const char *t_str = NULL;
	switch(lua_type(sta->L, -1))
	{
	case LUA_TNIL:
		v->vtype = QBS_NIL;
		break;
	case LUA_TNUMBER:
		v->vtype = QBS_NUMBER;
		v->values.nval = lua_tonumber(sta->L, -1);
		break;
	case LUA_TSTRING:
		v->vtype = QBS_STRING;
		t_str = lua_tostring(sta->L, -1);
		v->values.sval.str = (char*)malloc(strlen(t_str)+1);
		v->values.sval.len = strlen(t_str);
		strcpy(v->values.sval.str, t_str);
		break;
	case LUA_TBOOLEAN:
		v->vtype = QBS_BOOLEAN;
		v->values.bval = lua_toboolean(sta->L, -1);
		break;
	case LUA_TTABLE:
		v->vtype = QBS_TABLE;
		v->values.table = table_create(sta);
		break;
	default:
		v->vtype = QBS_NIL;
		luaL_error(sta->L, "not support this return type");
		break;
	}
	/* do not pop the element here */
	return v;
}

/**
*	qbase lua implements
**/
void
qbase_lua_create(qbase_sta **sta_ptr)	{
	if(_stapool.has_init != 1)
		pool_initialize();
	*(sta_ptr) = sta_create();
}

void
qbase_lua_free(qbase_sta *sta)	{
	sta_release(sta);
}

qbase_value*
qbase_lua_get(qbase_sta *sta, const char *name)	{
	qbase_value *v = NULL;
	int stackcnt = 0;
	lua_getglobal(sta->L, name);
	v = get_luavalue(sta);
	// if v is table, set it's name
	if(v->vtype == QBS_TABLE)   {
        v->values.table->fieldnames[0] = (char*)malloc(strlen(name)+1);
        strcpy(v->values.table->fieldnames[0], name);
    }
	stackcnt = lua_gettop(sta->L);
	lua_pop(sta->L, stackcnt);
	return v;
}

void
qbase_lua_freetable(qbase_table *tbl)	{
	table_drop(tbl);
	free(tbl);
}

qbase_value*
qbase_lua_getfield(qbase_sta *sta, qbase_table *tb, const char *field)	{
	qbase_value *v = NULL;
	int stackcnt = 0;
	if(tb == NULL || tb->field_count <= 0 || tb->fieldnames == NULL)
		return NULL;
	lua_getglobal(sta->L, tb->fieldnames[0]);
	lua_pushstring(sta->L, field);
	lua_gettable(sta->L, -2);
	v = get_luavalue(sta);
	if(v->vtype == QBS_NIL)	{
		free(v);
		v = NULL;
	}
	// check the stack
	stackcnt = lua_gettop(sta->L);
	lua_pop(sta->L, stackcnt);
	return v;
}

void
qbase_lua_reg(qbase_sta *sta, const char *name, qbase_regfunc f)	{
	lua_register(sta->L, name, f);
}

qbase_rets
qbase_lua_load(qbase_sta *sta, const char *buf, size_t retcnt, int from)	{
	FILE *f = NULL;
	qbase_rets v = NULL;
	int i, stackcnt = 0;
	int buffer_size = 0;
	int has_ret = 0;
	char *buffer = NULL;
	switch(from)
	{
	case FROM_BUFFER:
		/*	the buffer structure is :  [buffersize(4 Bytes)][buffer(buffersize Bytes)]	*/
		buffer_size = *((int*)buf);
		buffer = (char*)(buf+sizeof(int));
		if(luaL_loadbuffer(sta->L,buffer, buffer_size, "bufchunk") == 0 || lua_pcall(sta->L, 0, retcnt, 0) == 0)	{
			has_ret = 1;
		}
		break;
	case FROM_FILE:
		f = fopen(buf, "r");
		if(f != NULL && luaL_dofile(sta->L, buf) == 0)	{
            fclose(f);
			has_ret = 1;
		}
		break;
	default:
		break;
	}
	if(retcnt > 0)	{
        if(has_ret) {
            v = (qbase_rets)malloc(sizeof(qbase_value*)*retcnt);
            for(i = 0; i < retcnt; i++) {
                v[i] = get_luavalue(sta);
                lua_pop(sta->L, 1);
            }
        }
	}
	stackcnt = lua_gettop(sta->L);
	lua_pop(sta->L,stackcnt);
	return v;
}

qbase_rets
qbase_lua_call(qbase_sta *sta, const char *fname, int paramcnt, qbase_value *v, int retcnt)	{
    qbase_rets rv = NULL;
    int i, stackcnt = 0;
	lua_getglobal(sta->L, fname);
	if(!lua_isfunction(sta->L, -1)) {
        return NULL;
	}
	if(paramcnt > 0)    {
        for(i = 0; i < paramcnt; i++)   {
            switch(v->vtype)
            {
            case QBS_NIL:
                lua_pushnil(sta->L);
                break;
            case QBS_BOOLEAN:
                lua_pushboolean(sta->L, v->values.bval);
                break;
            case QBS_NUMBER:
                lua_pushnumber(sta->L, v->values.nval);
                break;
            case QBS_STRING:
                lua_pushstring(sta->L, v->values.sval.str);
                break;
            case QBS_TABLE:
            case QBS_UNKNOW:
            default:
                break;
            }
        }
	}
	if(lua_pcall(sta->L, 0, retcnt, 0) == 0)    {
        if(retcnt>0)    {
            rv = (qbase_rets)malloc(sizeof(qbase_value*)*retcnt);
            for(i = 0; i < retcnt; i++) {
                rv[i] = get_luavalue(sta);
                lua_pop(sta->L, 1);
            }
        }
	}
	stackcnt = lua_gettop(sta->L);
	lua_pop(sta->L,stackcnt);
	return rv;
}

qbase_rets
qbase_lua_exec(qbase_sta *sta, const char *text, int retcnt)	{
	int stackcnt = 0, i;
	qbase_rets v = NULL;
	if(!luaL_dostring(sta->L, text))	{
		if(retcnt > 0)	{
			v = (qbase_rets)malloc(sizeof(qbase_value*)*retcnt);
			for(i = 0; i < retcnt; i++)	{
				v[i] = get_luavalue(sta);
				lua_pop(sta->L, 1);
			}
		}
	}
	lua_gc(sta->L, LUA_GCCOLLECT, 0);
	stackcnt = lua_gettop(sta->L);
	lua_pop(sta->L,stackcnt);
	return v;
}
