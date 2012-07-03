#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int m[] = {1, 2, 3, 4};

int dot()
{
	return m[0]*m[3]-m[1]*m[2];
}

int sum()
{
	int sum=0, i;
	for(i=0; i<4; i++)
		sum+=m[i];
	return sum;
}