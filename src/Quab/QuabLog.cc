#include "debug/QuabLog.h"
#include "qbase/qbase.h"
using namespace Quab;

/*    QuabLog implements    */
void QuabLog::Log(const char *moduleName, const char *msg)	{
	qbase_log(moduleName, msg);
}

void QuabLog::Error(const char *moduleName, const char *msg)	{
	qbase_error(moduleName, msg);
}