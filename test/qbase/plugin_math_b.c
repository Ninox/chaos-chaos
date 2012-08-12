#include <stdio.h>
#include <stdlib.h>

static int m[] = {1, 2, 3, 4};

#ifdef QBASE_MSVC
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API
#endif

int PLUGIN_API dot()
{
	return m[0]*m[3]-m[1]*m[2];
}

int PLUGIN_API sum()
{
	int sum=0, i;
	for(i=0; i<4; i++)
		sum+=m[i];
	return sum;
}
