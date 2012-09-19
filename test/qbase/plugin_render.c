#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef QBASE_MSVC
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API
#endif
void PLUGIN_API renderit()
{
	printf("i render a square here ~~~~~\n");
}
