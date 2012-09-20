#ifndef QUAB_DEBUG_LOG_H
#define QUAB_DEBUG_LOG_H

#include "../QuabDef.h"
#define QUAB_DEFAULT_LOG_DIR "./log/"

namespace Quab
{
    class QUAB_API QuabLog
	{
	public:		
		static void Log(const char *msg);		
		static void Error(const char *msg);
	};	
}

#endif // QuabLog.h
