#ifndef QUAB_PREDEFINATION_H
#define QUAB_PREDEFINATION_H

#define interface class

#ifdef QUAB_USE_DOUBLE
typedef double Real;
#else
typedef float Real;
#endif

// platform select
#ifdef QUAB_OS_LINUX
#   define QUAB_API
#else
#   ifdef QUAB_LIBRARY
#	    define QUAB_API __declspec(dllexport)
#	else
#		define QUAB_API __declspec(dllimport)
#	endif
#endif
#ifdef QUAB_OS_MAC
#	define QUAB_API
#endif

#endif
