#include <basedebug.h>
#include <basepacker.h>
#include <basescript.h>
#include <baseloader.h>
#include <basemath.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define ES 0.005f

#ifdef OS_WIN32
#define XALOY_API LUALIB_API __declspec(dllexport)
#else
#define XALOY_API
#endif

/*   helper for this binding to get table   */
static void
set_vector(lua_State *L, qbase_vector *v)	{
	lua_pushnumber(L, v->posX);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, v->posY);
	lua_setfield(L, -2, "y");
}

static void
get_vector(lua_State *L, qbase_vector *v)    {
    lua_getfield(L, -1, "x");
    v->posX = (float)lua_tonumber(L,-1);
    lua_pop(L, 1);
    lua_getfield(L, -1, "y");
    v->posY = (float)lua_tonumber(L,-1);
    lua_pop(L, 1);
}

static void
set_matrix(lua_State *L, qbase_matrix2 *mtx)	{
	int i;
	for(i = 0; i < 4; i++)	{
		lua_pushnumber(L, mtx->m[i/2][i%2]);
		lua_rawseti(L, -2, i+1);
	}
}

static void
get_matrix(lua_State *L, qbase_matrix2 *mtx)    {
	int i;
	for(i = 0; i < 4; i++)	{
		lua_rawgeti(L, -1, i+1);
		mtx->m[i/2][i%2] = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);
	}
}

static void
set_qbasevalue(lua_State *L, qbase_value *v)	{
	int ret;
	switch(v->vtype)	
	{
	default:
	case QBS_NIL:
		ret = 0;
	case QBS_NUMBER:
		lua_pushnumber(L, v->values.nval);
		ret = 1;
	case QBS_BOOLEAN:
		lua_pushboolean(L, v->values.bval);
		ret = 1;
	case QBS_STRING:
		lua_pushstring(L, v->values.sval.str);
		ret = 1;
	case QBS_TABLE:	
		lua_getglobal(L, v->values.table->fieldnames[0]);
		ret = 1;
	}
	free(v);
	return ret;
}

/*---------- basemath API binding ----------*/
static int
qmath_todegree(lua_State *L)    {
    double rad = lua_tonumber(L, -1);
    lua_pushnumber(L, qbase_math_todegree((float)rad));
    return 1;
}

static int
qmath_equal(lua_State *L)    {
    float n1 = (float)lua_tonumber(L, 1);
    float n2 = (float)lua_tonumber(L, 2);
    lua_pushinteger(L, qbase_math_equal(n1, n2));
    return 1;
}

static int
qvector_plus(lua_State *L)    {    
	qbase_vector v1, v2, ret;
    get_vector(L, &v2);
    lua_pushvalue(L, -2);
    get_vector(L, &v1);
    lua_pop(L, 1);

    /*    create a new table    */
	ret = qbase_vector_plus(&v1, &v2);
    lua_newtable(L);
    set_vector(L, &ret);
    return 1;
}

static int
qvector_dot(lua_State *L)    {
    qbase_vector v1, v2;

    get_vector(L, &v2);
    lua_pushvalue(L, -2);
    get_vector(L, &v1);
    lua_pop(L, 1);

    /*    calculate the dot operation    */
    lua_pushnumber(L, qbase_vector_dot(&v1, &v2));
    return 1;
}

static int
qvector_cos(lua_State *L)    {
    qbase_vector v1, v2;

    get_vector(L, &v2);
    lua_pushvalue(L, -2);
    get_vector(L, &v1);
    lua_pop(L, 1);

    /*    calculate the dot operation    */
    lua_pushnumber(L, qbase_vector_cos(&v1, &v2));
    return 1;
}

static int
qvector_sin(lua_State *L)    {
    qbase_vector v1, v2;

    get_vector(L, &v2);
    lua_pushvalue(L, -2);
    get_vector(L, &v1);
    lua_pop(L, 1);

    /*    calculate the dot operation    */
    lua_pushnumber(L, qbase_vector_sin(&v1, &v2));
    return 1;
}

static int
qvector_isparallel(lua_State *L)    {
    qbase_vector v1, v2;

    get_vector(L, &v2);
    lua_pushvalue(L, -2);
	get_vector(L, &v1);
    lua_pop(L, 1);

    /*    calculate the dot operation    */
    lua_pushinteger(L, qbase_vector_isparallel(&v1, &v2));
    return 1;
}

static int
qvector_isvertical(lua_State *L)    {
    qbase_vector v1, v2;

    get_vector(L, &v2);
    lua_pushvalue(L, -2);
    get_vector(L, &v1);
    lua_pop(L, 1);

    /*    calculate the dot operation    */
    lua_pushinteger(L, qbase_vector_isvertical(&v1, &v2));
    return 1;
}

static int
qvector_length(lua_State *L)    {
    qbase_vector v;

    /*    calculate the dot operation    */
    get_vector(L, &v);
    lua_pushinteger(L, qbase_vector_length(&v));
    return 1;
}

static int
qvector_mul(lua_State *L)    {
    qbase_vector v;
	int ret;
	float k;
    /*    get the k value    */
    k = (float)lua_checknumber(L, -1);

    /*    get vector    */
    lua_pushvalue(L, -2);
    get_vector(L, &v);
    lua_pop(L, 1);    

    /*    get the qbase' API returns */
    ret = qbase_vector_mul(&v, k);
    lua_newtable(L);
    set_vector(L, &ret);
	lua_pushinteger(L, ret);
    return 2;
}

static int
qmatrix_init(lua_State *L)    {
	qbase_matrix2 mtx, ret;
	get_matrix(L, &mtx);
	qbase_matrix_init(&ret, 0, mtx.m[0][0],mtx.m[0][1],mtx.m[1][0],mtx.m[1][1]);

	lua_newtable(L);
	set_matrix(L, &ret);
	return 1;
}

static int
qmatrix_zero(lua_State *L)    {
	qbase_matrix2 mtx;
	int result = -1;
	qbase_matrix_zero(&mtx, 0);

	if(mtx.m[0][0] == 0 && mtx.m[0][1] == 0 && mtx.m[1][0] == 0 && mtx.m[1][1] == 0)	{
		result = 1;
	}
	else result = 0;
	lua_pushboolean(L, result);

	return 1;
}

static int
qmatrix_plus(lua_State *L)    {
	int i;
	qbase_matrix2 mtx1, mtx2, ret;
	/*	get matrix data	*/
	get_matrix(L, &mtx2);
	lua_pushvalue(L, -2);
	get_matrix(L, &mtx1);
	lua_pop(L, 1);

	/*  calculate the result and return it  */
	ret = qbase_matrix_plus(&mtx1, &mtx2);
	lua_newtable(L);
	set_matrix(L, &ret);

	return 1;
}

static int
qmatrix_trans(lua_State *L)    {
	qbase_matrix2 mtx;
	/*  get matrix and call trans function  */
	get_matrix(L, &mtx);
	qbase_matrix_trans(&mtx);

	/*  return matrix  */
	lua_newtable(L);
	set_matrix(L, &mtx);

	return 1;
}

static int
qmatrix_adjoint(lua_State *L)    {
	qbase_matrix2 mtx, ret;
	/*  get matrix and call trans function  */
	get_matrix(L, &mtx);
	ret = qbase_matrix_adjoint(&mtx);

	/*  return matrix  */
	lua_newtable(L);
	set_matrix(L, &ret);

	return 1;
}

static int
qmatrix_inverse(lua_State *L)    {
	qbase_matrix2 mtx;
	/*  get matrix and call trans function  */
	get_matrix(L, &mtx);
	qbase_matrix_inverse(&mtx);

	/*  return matrix  */
	lua_newtable(L);
	set_matrix(L, &mtx);

	return 1;
}

static int
qmatrix_det(lua_State *L)    {
	qbase_matrix2 mtx;
	float det;

	/*  get matrix data and calculate the det  */
	get_matrix(L, &mtx);
	det = qbase_matrix_det(&mtx);

	/*  return the det value  */
	lua_pushnumber(L, det);
	return 1;
}

static int
qmatrix_extend(lua_State *L)    {
	qbase_matrix2 mtx;
	float k;
	/*  the lua table is the first parameters of this function  */
	lua_pushvalue(L, -2);
	get_matrix(L, &mtx);
	lua_pop(L, 1);
	k = lua_tonumber(L, -1);
	qbase_matrix_extend(&mtx, k);

	/*  return matrix data  */
	lua_newtable(L);
	set_matrix(L, &mtx);

	return 1;
}

static int
qmatrix_mul(lua_State *L)    {
	qbase_matrix2 mtx1, mtx2, ret;

	/*  get matrix data  */
	get_matrix(L, &mtx2);
	lua_pushvalue(L, -2);
	get_matrix(L, &mtx1);
	lua_pop(L, 1);

	/*  return the mul operation's return  */
	ret = qbase_matrix_mul(&mtx1, &mtx2);
	lua_newtable(L);
	set_matrix(L, &ret);

	return 1;
}

static int
qmatrix_vmul(lua_State *L)    {
	qbase_vector v, ret;
	qbase_matrix2 mtx;

	/*  get arguments data  */
	get_matrix(L, &mtx);
	lua_pushvalue(L, -2);
	get_vector(L, &v);
	lua_pop(L, 1);
	ret = qbase_matrix_vmul(&v, &mtx);

	/*  return the vmul result  */
	lua_newtable(L);
	set_vector(L, &ret);

	return 1;
}

/*---------- baseloader API binding ----------*/
static int
qloader_init(lua_State *L)	{
	const char *name = lua_checkstring(L, -1);
	qbase_loader_init(name);
	lua_pushboolean(L, 1);
	return 1;
}

static int
qloader_free(lua_State *L)	{
	const char *name = lua_checkstring(L, -1);
	qbase_loader_init(name);
	qbase_loader_free(name);
	lua_pushboolean(L, 1);
	return 1;
}

static int
qloader_getf(lua_State *L)	{
	const char *module_name = lua_checkstring(L, -2);
	const char *func_name = lua_checkstring(L, -1);
	qbase_loader_init(module_name);
	void *f = qbase_loader_getf(module_name, func_name);
	lua_pushboolean(L, f == NULL? 0 : 1);
	return 1;
}

static int
qloader_destory(lua_State *L)	{
	int i, cnt;
	const char **name_table = NULL;
	cnt = lua_checknumber(L, -2);	
	name_table = (const char**)malloc(sizeof(char*) * cnt);	
	for(i = 0; i < cnt; i++)	{
		lua_rawgeti(L, -1, i+1);
		name_table[i] = lua_checkstring(L, -1);
		qbase_loader_init(name_table[i]);
		lua_pop(L, 1);
	}
	// free name_table
	free(name_table);
	qbase_loader_destory();
	return 1;
}

/*---------- basescript API binding ----------*/ 
static qbase_sta *sta = NULL;
static int
push_functions(lua_State *L)	{
	printf("Hello world\n");
	return 0;
}

static int
qlua_create(lua_State *L)	{
	qbase_lua_create(&sta);
	lua_pushboolean(L, sta == NULL? 0 : 1);
	qbase_lua_free(&sta);
	if(sta != NULL)
		free(sta);
	return 1;
}

static int
qlua_free(lua_State *L)	{
	qbase_lua_create(&sta);
	qbase_lua_free(&sta);
	lua_pushboolean(L, sta == NULL ? 1 : 0);
	return 1;
}

static int
qlua_get(lua_State *L)	{
	const char *example_src = lua_checkstring(L, -2);
	const char *varname = lua_checkstring(L, -1);
	int ret = 0;
	qbase_value *v = NULL;
	qbase_lua_create(&sta);
	qbase_lua_load(sta, example_src, 0, FROM_FILE);
	v = qbase_lua_get(sta, varname);
	set_qbasevalue(L, v);
	v = NULL;
	qbase_lua_free(&sta);
	return ret;
}

static int
qlua_freetable(lua_State *L)	{
	const char *src = lua_checkstring(L, -2);
	const char *tbname = lua_checkstring(L, -1);
	qbase_value *v = NULL;
	qbase_lua_create(&sta);
	qbase_lua_load(sta, src, 0, FROM_FILE);
	v = qbase_lua_get(sta, tbname);
	
	if(v->vtype != QBS_TABLE)	{
		lua_pushboolean(L, 0);		
	}
	else	{
		qbase_lua_freetable(&v->values.table);
		if(v->values.table == NULL)	{
			lua_pushboolean(L, 1);
		} else	{
			lua_pushboolean(L, 0);
		}
		free(v);
	}
	qbase_lua_free(&sta);
	return 1;
}

static int
qlua_getfield(lua_State *L)	{
	const char *src = lua_checkstring(L, -3);
	const char *tbname = lua_checkstring(L, -2);
	const char *field = lua_checkstring(L, -1);
	qbase_value *v = NULL, *tv = NULL;

	qbase_lua_create(&sta);
	qbase_lua_load(sta, src, 0, FROM_FILE);
	v = qbase_lua_get(sta, tbname);
	tv = qbase_lua_getfield(sta, v->values.table, field);
	set_qbasevalue(L, tv);
	qbase_lua_freetable(&v->values.table);
	free(v);
	free(tv);	
	v = NULL;
	tv = NULL;
	return 1;
}

static int
qlua_reg(lua_State *L)	{
	qbase_lua_create(&sta);
	qbase_lua_reg(sta, "sayhello", push_functions);
	qbase_lua_exec(sta, "sayhello()", 0);
	lua_pushboolean(L, 1);
	return 1;
}

static int
qlua_load(lua_State *L)	{
	return 0;
}

static int
qlua_call(lua_State *L)	{
	return 0;
}

static int
qlua_exec(lua_State *L)	{
	return 0;
}


/*---------- basepacker API binding ----------*/
static int
qpacker_create(lua_State *L)	{
	return 0;
}

static int
qpacker_load(lua_State *L)	{
	return 0;
}

static int
qpacker_save(lua_State *L)	{
	return 0;
}

static int
qpacker_free(lua_State *L)	{
	return 0;
}

static int
qpacker_setsercurity(lua_State *L)	{
	return 0;
}

static int
qpacker_setpwd(lua_State *L)	{
	return 0;
}

static int
qpacker_show(lua_State *L)	{
	return 0;
}

static int
qpacker_get(lua_State *L)	{
	return 0;
}

static int
qpacker_add(lua_State *L)	{
	return 0;
}

static int
qpacker_remove(lua_State *L)	{
	return 0;
}

static int
qpacker_update(lua_State *L)	{
	return 0;
}

static int
qpacker_rename(lua_State *L)	{
	return 0;
}

/*----------------------  register to Lua ----------------------*/
static const
struct luaL_Reg qbase_apis[] = {
    {"qmath_todegree",qmath_todegree},
    {"qmath_equal",qmath_equal},
    {"qvector_plus",qvector_plus},
    {"qvector_dot", qvector_dot},
    {"qvector_cos", qvector_cos},
    {"qvector_sin",qvector_sin},
    {"qvector_isparallel", qvector_isparallel},
    {"qvector_isvertical", qvector_isvertical},
    {"qvector_length", qvector_length},
    {"qvector_mul", qvector_mul},
    {"qmatrix_init", qmatrix_init},
    {"qmatrix_zero", qmatrix_zero},
    {"qmatrix_plus", qmatrix_plus},
    {"qmatrix_trans", qmatrix_trans},
    {"qmatrix_adjoint", qmatrix_adjoint},
    {"qmatrix_inverse", qmatrix_inverse},
    {"qmatrix_det", qmatrix_det},
    {"qmatrix_extend", qmatrix_extend},
    {"qmatrix_mul",qmatrix_mul},
    {"qmatrix_vmul",qmatrix_vmul},
    {"qloader_init",qloader_init},
    {"qloader_free",qloader_free},
    {"qloader_getf",qloader_getf},
    {"qloader_destory",qloader_destory},
    {"qlua_create",qlua_create},
    {"qlua_free",qlua_free},
    {"qlua_get",qlua_get},
    {"qlua_freetable",qlua_freetable},
    {"qlua_getfield",qlua_getfield},
    {"qlua_reg",qlua_reg},
    {"qlua_load",qlua_load},
    {"qlua_call",qlua_call},
    {"qlua_exec",qlua_exec},
    {"qpacker_create",qpacker_create},
    {"qpacker_load",qpacker_load},
    {"qpacker_save",qpacker_save},
    {"qpacker_free",qpacker_free},
    {"qpacker_setsercurity",qpacker_setsercurity},
    {"qpacker_setpwd",qpacker_setpwd},
    {"qpacker_show",qpacker_show},
    {"qpacker_get",qpacker_get},
    {"qpacker_add",qpacker_add},
    {"qpacker_remove",qpacker_remove},
    {"qpacker_update",qpacker_update},
    {"qpacker_rename",qpacker_rename},
    {NULL, NULL}
};

XALOY_API int
luaopen_qbase(lua_State *L)    {
    lua_newtable(L);
    luaL_setfuncs(L, qbase_apis, 0);
    return 1;
}