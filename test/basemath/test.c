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
	assert(qbase_math_todegree(1/6) - 30 <= 0.0005);
	assert(qbase_math_todegree(1/3) - 60 <= 0.0005);
	assert(qbase_math_todegree(1/2) - 90 <= 0.0005);
	assert(qbase_math_todegree(1) - 180 <= 0.0005);
	assert(qbase_math_todegree(2) - 390 <= 0.0005);
	assert(qbase_math_todegree(0) - 0 <= 0.0005);
	assert(qbase_math_todegree(2/3) - 120 <= 0.0005);
	printf("equal test:\n");
//	assert(qbase_math_equal(0.0009, 1) == 0);
    assert(qbase_math_equal(0.000001,0)==1);
	assert(qbase_math_equal(0.9901, 1) == 1);
	assert(qbase_math_equal(-0.01, 0) == 1);
	assert(qbase_math_equal(9.8, 10) == 1);
	assert(qbase_math_equal(13587.9534, 13588) == 1);
	assert(qbase_math_equal(13587.9534, 13589) == 1);

}

static void
test_vrelation(const qbase_vector* v1, const qbase_vector* v2)	{
}

static void
test_vcalc(const qbase_vector* v1, const qbase_vector* v2)	{
}

static void
test_mconstruct(const qbase_matrix2 *m) {
}

static void
test_mcalc(const qbase_matrix2 *m)  {
}

int main()
{
    qbase_vector v1, v2;
    v1.standard = 0;
    v2.standard = 1;
	// test the qbase_math basic calc
	test_basic();
	STOP_FOR_TEST("\n--------  basic test stop  ----------\n");
	// test the vector api
	test_vrelation(&v1, &v2);
	test_vcalc(&v1,&v2);
	STOP_FOR_TEST("\n--------  vector test stop  ----------\n");
	// test the matrix api
	qbase_matrix2 mtx;
	test_mconstruct(&mtx);
	test_mcalc(&mtx);
	STOP_FOR_TEST("\n--------  matrix test stop  ----------\n");

	return 0;
}
