#include <stdio.h>
#include <stdlib.h>

#ifdef QBASE_MSVC
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API
#endif

void PLUGIN_API say_somthing(const char *sth)
{
	printf("i wanna to say sth:'%s'\n", sth);
}
