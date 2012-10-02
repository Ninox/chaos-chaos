#ifndef QUAB_DEBUG_LOG_H
#define QUAB_DEBUG_LOG_H

#include "../QuabDef.h"

namespace Quab
{
    class QUAB_API QuabLog
	{
	public:		
		static void log(const char *moduleName, const char *msg);		
		static void error(const char *moduleName, const char *msg);
	};	
}

#endif // QuabLog.h
