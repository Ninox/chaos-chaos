#include <stdio.h>

#ifdef QBASE_MSVC
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API
#endif

int PLUGIN_API add(int a, int b)
{
	return a+b;
}

int PLUGIN_API min(int a, float b)
{
	return a - (float)b;
}
