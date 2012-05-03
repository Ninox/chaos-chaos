#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "basemath.h"

#define STOP_FOR_TEST(str)	\
	printf((str));	\
	getchar()

static void
test_basic()	{
}

static void
test_vrelation(const qbase_vector* v1, const qbase_vector* v2)	{
}

static void
test_vcalc(const qbase_vector* v1, const qbase_vector* v2)	{
}

static void 

int main()	
{
	// test the qbase_math basic calc
	test_basic();
	STOP_FOR_TEST("\n--------  basic test stop  ----------\n");
	// test the vector api
	test_vrelation(&v1, &v2);
	test_vcalc(v1,v2);
	STOP_FOR_TEST("\n--------  vector test stop  ----------\n");
	// test the matrix api
	qbase_matrix2 mtx;
	test_mconstruct(&mtx);
	test_mcalc(&mtx);
	STOP_FOR_TEST("\n--------  matrix test stop  ----------\n");
	
	return 0;
}