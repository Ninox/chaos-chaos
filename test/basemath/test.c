#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "basemath.h"

#define STOP_FOR_TEST(str)	\
	printf((str));	\
	getchar()

static void
test_basic()	{
	printf("degree test:\n");
	printf("%s\n",qbase_math_todegree(1/6) - 30 <= 0.0005?"true":"false");
	printf("%s\n",qbase_math_todegree(1/3) - 60 <= 0.0005?"true":"false");
	printf("%s\n",qbase_math_todegree(1/2) - 90 <= 0.0005?"true":"false");
	printf("%s\n",qbase_math_todegree(1) - 180 <= 0.0005?"true":"false");
	printf("%s\n",qbase_math_todegree(2) - 390 <= 0.0005?"true":"false");
	printf("%s\n",qbase_math_todegree(0) - 0 <= 0.0005?"true":"false");
	printf("%s\n",qbase_math_todegree(2/3) - 120 <= 0.0005?"true":"false");
	printf("equal test:\n");
//	assert(qbase_math_equal(0.0009, 1) == 0);
    printf("%s\n",qbase_math_equal(0.000001,0)==1?"true":"false");
	printf("%s\n",qbase_math_equal(0.9901, 1) == 1?"true":"false");
	printf("%s\n",qbase_math_equal(-0.01, 0) == 1?"true":"false");
	printf("%s\n",qbase_math_equal(9.8, 10) == 1?"true":"false");
	printf("%s\n",qbase_math_equal(13587.9534, 13588) == 1?"true":"false");
	printf("%s\n",qbase_math_equal(13587.9534, 13589) == 1?"true":"false");

}

static void
test_vrelation()	{
	qbase_vector v1, v2;
	v1.standard = 0;
	v2.standard = 1;
	v1.posX = 0; v1.posX = 1;
	printf("aaa %f  %f\n",v1.posX,v1.posY);
	printf("aaa %f  %f\n",v2.posX,v2.posY);
	printf("%s\n",qbase_vector_isparallel(&v1,&v2)==1?"true":"false");
	printf("%s\n",qbase_vector_isvertical(&v1,&v2)==1?"true":"false");
	v1.posX = 1; v1.posX = 1;
	v2.posX = -1; v2.posX = 1;
	printf("%s\n",qbase_vector_isparallel(&v1,&v2)==1?"true":"false");
	printf("%s\n",qbase_vector_isvertical(&v1,&v2)==1?"true":"false");
	v1.posX = 3; v1.posX = 1;
	v2.posX = 1; v2.posX = 6;
	printf("%s\n",qbase_vector_isparallel(&v1,&v2)==1?"true":"false");
	printf("%s\n",qbase_vector_isvertical(&v1,&v2)==1?"true":"false");
	v1.posX = 2; v1.posX = 6;
	v2.posX = 9; v2.posX = -3;
	printf("%s\n",qbase_vector_isparallel(&v1,&v2)==1?"true":"false");
	printf("%s\n",qbase_vector_isvertical(&v1,&v2)==1?"true":"false");
	v1.posX = 0; v1.posX = 1;
	v2.posX = 1; v2.posX = 0;
	printf("%s\n",qbase_vector_isparallel(&v1,&v2)==1?"true":"false");
	printf("%s\n",qbase_vector_isvertical(&v1,&v2)==1?"true":"false");
}

static void
test_vcalc()	{
}

static void
test_mconstruct() {
}

static void
test_mcalc()  {
}

int main()
{
	// test the qbase_math basic calc
	test_basic();
	STOP_FOR_TEST("\n--------  basic test stop  ----------\n");
	// test the vector api
	test_vrelation();
	test_vcalc();
	STOP_FOR_TEST("\n--------  vector test stop  ----------\n");
	// test the matrix api
	test_mconstruct();
	test_mcalc();
	STOP_FOR_TEST("\n--------  matrix test stop  ----------\n");

	return 0;
}
