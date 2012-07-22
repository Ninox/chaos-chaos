#ifndef QBASE_LUA_H
#define QBASE_LUA_H
#include <stddef.h>
#include <lua.h>

enum qbase_vtypes	{
	QBS_NIL 	= 0,
	QBS_NUMBER 	= 1,
	QBS_BOOLEAN	= 2,
	QBS_STRING 	= 3,
	QBS_TABLE 	= 4,
	QBS_UNKNOW 	= 5
};
enum qbase_buftype	{
	FROM_BUFFER = 0,
	FROM_FILE = 1
};

typedef struct qbase_table  {
	// the fieldnames[0] means table name
	char **fieldnames;
	size_t field_count;
} qbase_table;

typedef struct qbase_string	{
	char *str;
	size_t len;
} qbase_string;

typedef struct qbase_value		{
	int vtype;
	union	{
		int	bval;
		double nval;
		qbase_string sval;
		qbase_table *table;
	} values;
} qbase_value;

typedef struct qbase_sta qbase_sta;
typedef lua_CFunction qbase_regfunc;
typedef qbase_value** qbase_rets;

/**
*	qbase lua initalization functions
**/

/** \brief initalize the qbase lua's object
 *
 * \param the address of the object's pointer, qbase_sta**
 * \return void
 *
 */
void qbase_lua_create(qbase_sta **sta_ptr);

/** \brief release the qbase lua's object
 *
 * \param the address of the object's pointer, qbase_sta**
 * \return void
 *
 */
void qbase_lua_free(qbase_sta **sta);

/**
*	get lua's variable
**/

/** \brief get a global value in object
 *
 * \param qbase lua's object, qbase_sta*
 * \param global variant's name, const char*
 * \return a qbase script return value
 *
 */
qbase_value* qbase_lua_get(qbase_sta *sta, const char *name);

/** \brief release a table value
 *
 * \param the table object, qbase_table**
 * \return void
 *
 */
void qbase_lua_freetable(qbase_table **tbl);

/** \brief get field value in a table
 *
 * \param qbase lua's object qbase_sta*
 * \param table object, qbase_table*
 * \param field name, const char*
 * \return a qbase script return object
 *
 */
qbase_value* qbase_lua_getfield(qbase_sta *sta, qbase_table *tb, const char *field);

/**
*	function executer
**/

/** \brief register a functions into lua state
 *
 * \param qbase lua's object, qbase_sta*
 * \param the function name to register, const char*
 * \param the registed function, qbase_regfunc
 * \return void
 *
 */
void qbase_lua_reg(qbase_sta *sta, const char *name, qbase_regfunc f);

/** \brief load and execute a lua's stream buffer or file
 *
 * \param qbase lua's object, qbase_sta*
 * \param buffer or file path, const char*
 * \param how many return of this buffer/file, size_t
 * \param FROM_FILE or FROM_BUFFER, int
 * \return a qbase lua's return value
 *
 */
qbase_rets qbase_lua_load(qbase_sta *sta, const char *buf, size_t retcnt, int from);

/** \brief call a function which in the lua state
 *
 * \param qbase lua's object, qbase_sta*
 * \param function name, const char*
 * \param the number of parameters int
 * \param th parameters' values, qbase_value*
 * \param return count, int
 * \return the returns of qbase value
 *
 */
qbase_rets qbase_lua_call(qbase_sta *sta, const char *fname, int paramcnt, qbase_value *v, int retcnt);

/** \brief execute a script text
 *
 * \param qbase lua's object, qbase_sta*
 * \param script,text const char*
 * \param return count, int
 * \return return values
 *
 */
qbase_rets qbase_lua_exec(qbase_sta *sta, const char *text, int retcnt);

#endif
