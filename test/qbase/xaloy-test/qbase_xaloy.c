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
get_vector(lua_State *L, float *x, float *y)    {
    lua_pushstring(L, "x");
    lua_gettable(L, -2);
    *x = (float)lua_tonumber(L,-1);
    lua_pop(L, 1);
    lua_pushstring(L, "y");
    lua_gettable(L, -2)
    *y = (float)lua_tonumber(L,-1);
    lua_pop(L, 1);
}

static void
get_matrix(lua_State *L, float *m)    {
	int i;
	for(i = 0; i < 4; i++)	{
		lua_rawgeti(L, -1, i+1);
		m[i] = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);
	}
}

/*---------- basemath API binding ----------*/
static int
qmath_todegree(lua_State *L)    {
    double rad = lua_tonumber(L, -1);
    lua_pushnumber(L, qbase_math_todegree((float)rad);
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
    float v1x, v1y, v2x, v2y;
    
    get_vector(L, &v2x, &v2y);
    lua_pushvalue(L, -2);
    
    get_vector(L, &v1x, &v1y);
    lua_pop(L, 1);
    
    /*    create a new table    */
    lua_newtable(L);
    lua_pushstring(L, "x");
    lua_pushnumber(L, v1x+v2x);
    lua_pushstring(L, "y");
    lua_pushnumber(L, v1x+v2x);
    lua_settable(L,-5);
    return 1;
}

static int
qvector_dot(lua_State *L)    {
    float v1x, v1y, v2x, v2y;
    qbase_vector v1, v2;
    
    get_vector(L, &v2x, &v2y);
    lua_pushvalue(L, -2);
    
    get_vector(L, &v1x, &v1y);
    lua_pop(L, 1);
    
    /*    calculate the dot operation    */    
    v1.posX = v1x; v1.posY = v1y;
    v2.posX = v2x; v2.posY = v2y;
    lua_pushnumber(L, qbase_vector_dot(v1, v2))
    return 1;
}

static int
qvector_cos(lua_State *L)    {
    float v1x, v1y, v2x, v2y;
    qbase_vector v1, v2;
    
    get_vector(L, &v2x, &v2y);
    lua_pushvalue(L, -2);
    
    get_vector(L, &v1x, &v1y);
    lua_pop(L, 1);
    
    /*    calculate the dot operation    */    
    v1.posX = v1x; v1.posY = v1y;
    v2.posX = v2x; v2.posY = v2y;
    lua_pushnumber(L, qbase_vector_cos(v1, v2))
    return 1;
}

static int
qvector_sin(lua_State *L)    {
    float v1x, v1y, v2x, v2y;
    qbase_vector v1, v2;
    
    get_vector(L, &v2x, &v2y);
    lua_pushvalue(L, -2);
    
    get_vector(L, &v1x, &v1y);
    lua_pop(L, 1);
    
    /*    calculate the dot operation    */    
    v1.posX = v1x; v1.posY = v1y;
    v2.posX = v2x; v2.posY = v2y;
    lua_pushnumber(L, qbase_vector_sin(v1, v2))
    return 1;
}

static int
qvector_isparallel(lua_State *L)    {
    float v1x, v1y, v2x, v2y;
    qbase_vector v1, v2;
    
    get_vector(L, &v2x, &v2y);
    lua_pushvalue(L, -2);
    
    get_vector(L, &v1x, &v1y);
    lua_pop(L, 1);
    
    /*    calculate the dot operation    */    
    v1.posX = v1x; v1.posY = v1y;
    v2.posX = v2x; v2.posY = v2y;
    lua_pushinteger(L, qbase_vector_isparallel(v1, v2))
    return 1;
}

static int
qvector_isvertical(lua_State *L)    {
    float v1x, v1y, v2x, v2y;
    qbase_vector v1, v2;
    
    get_vector(L, &v2x, &v2y);
    lua_pushvalue(L, -2);
    
    get_vector(L, &v1x, &v1y);
    lua_pop(L, 1);
    
    /*    calculate the dot operation    */    
    v1.posX = v1x; v1.posY = v1y;
    v2.posX = v2x; v2.posY = v2y;
    lua_pushinteger(L, qbase_vector_isvertical(v1, v2))
    return 1;
}

static int
qvector_length(lua_State *L)    {
    float v1x, v1y;
    qbase_vector v1;
    
    /*    calculate the dot operation    */        
    get_vector(L, &v1x, &v1y);    
    v1.posX = v1x; v1.posY = v1y;
    lua_pushinteger(L, qbase_vector_length(v1))
    return 1;
}

static int
qvector_mul(lua_State *L)    {
    float vx, vy, k, ret;
    qbase_vector v;
    /*    get the k value    */
    k = (float)lua_checknumber(L, -1);
    
    /*    get vector    */
    lua_pushvalue(L, -2);
    get_vector(L, &vx, &vy);
    lua_pop(L, 1);
    v.posX = vx; v.posY = vy;
    
    /*    get the qbase' API returns */
    ret = qbase_vector_mul(&v, k);
    lua_newtable(L);
    lua_pushstring(L, "x");
    lua_pushnumber(L, v.posX);
    lua_pushstring(L, "y");
    lua_pushnumber(L, v.posY);
    lua_settable(L, -5);
    lua_pushinteger(L, ret);
    
    return 2;    
}

static int
qmatrix_init(lua_State *L)    {
}

static int
qmatrix_zero(lua_State *L)    {
}

static int
qmatrix_plus(lua_State *L)    {
	int i;
	float m1[4], m2[4];
	qbase_matrix2 mtx1, mtx2, ret;
	/*	get matrix data	*/
	get_matrix(L, m2);
	lua_pushvalue(L, -2);
	get_matrix(L, m1);
	lua_pop(L, 1);
	
	/*	create matrix	*/	
	mtx1.m[0][0] = m1[0];mtx1.m[0][1] = m1[1];
	mtx1.m[1][0] = m1[2];mtx1.m[1][1] = m1[3];
	mtx2.m[0][0] = m2[0];mtx2.m[0][1] = m2[1];
	mtx2.m[1][0] = m2[2];mtx2.m[1][1] = m2[3];
	
	/*  calculate the result and return it  */
	ret = qbase_matrix_plus(&mtx1, &mtx2);
	lua_newtable(L);
	for(i = 0; i < 4; i++)	{
		lua_pushnumber(L, ret.m[i/2][i%2]);
		lua_rawseti(L, -2, i+1);
	}
	
	return 1;
}

static int
qmatrix_trans(lua_State *L)    {
}

static int
qmatrix_adjoint(lua_State *L)    {
}

static int
qmatrix_inverse(lua_State *L)    {
}

static int
qmatrix_det(lua_State *L)    {
}

static int
qmatrix_extend(lua_State *L)    {
}

static int
qmatrix_mul(lua_State *L)    {
}

static int
qmatrix_vmul(lua_State *L)    {
}

/*---------- baseloader API binding ----------*/
static int
qloader_init(lua_State *L)	{
	return 0;
}

static int
qloader_free(lua_State *L)	{
	return 0;
}

static int
qloader_getf(lua_State *L)	{
	return 0;
}

static int
qloader_destory(lua_State *L)	{
	return 0;
}

/*---------- basescript API binding ----------*/
static int
qlua_create(lua_State *L)	{
	return 0;
}

static int
qlua_free(lua_State *L)	{
	return 0;
}

static int
qlua_get(lua_State *L)	{
	return 0;
}

static int
qlua_freetable(lua_State *L)	{
	return 0;
}

static int
qlua_getfield(lua_State *L)	{
	return 0;
}

static int
qlua_reg(lua_State *L)	{
	return 0;
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
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {NULL, NULL}
};

XALOY_API int
luaopen_qbase(lua_State *L)    {
    lua_newtable(L);
    luaL_setfuncs(L, qbase_apis, 0);
    return 1;
}