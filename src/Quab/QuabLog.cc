#include "debug/QuabLog.h"
#include "qbase/qbase.h"
using namespace Quab;

/*    QuabLog implements    */
void QuabLog::log(const char *moduleName, const char *msg)	{
	qbase_log(moduleName, msg);
}

void QuabLog::error(const char *moduleName, const char *msg)	{
	qbase_error(moduleName, msg);
}