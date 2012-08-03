#ifndef QBASE_DEBUG_H
#define QBASE_DEBUG_H

/** \brief print a debug message
 *
 * \param the debug module's name, const char*
 * \param the output message, const char*
 * \return void
 *
 */
void qbase_debug(const char *obj, const char *msg);


/** \brief print a log message with time
 *
 * \param the object name about this log, const char*
 * \param the output message, const char*
 * \return void
 *
 */
void qbase_log(const char *obj, const char *msg);


/** \brief print a error message
 *
 * \param the error object name, const char*
 * \param the error message, const char*
 * \return void
 *
 */
void qbase_error(const char *obj, const char *msg);

#endif
