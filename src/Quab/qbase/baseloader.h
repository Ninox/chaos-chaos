#ifndef	QBASE_LOADER_H
#define QBASE_LOADER_H


/** \brief search the module name in CFG file, if the given path is ok, load the module in list
 *
 * \param module name, const char*
 * \return void
 *
 */
void qbase_loader_init(const char *name);


/** \brief release a module in list
 *
 * \param module name, const char*
 * \return void
 *
 */
void qbase_loader_free(const char *name);


/** \brief get function in the target module
 *
 * \param the target module's name, const char*
 * \param function name, const char*
 * \return void*
 *
 */
void* qbase_loader_getf(const char *name, const char *fname);


/** \brief destory all module in the list
 *
 * \return void
 *
 */
void qbase_loader_destory();

#endif
