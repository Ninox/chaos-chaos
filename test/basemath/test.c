#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "basemath.h"

#define PI 3.1415926f
#define STOP_FOR_TEST(str)	\
	printf((str));	\
	getchar()

static void
test_basic()	{
	assert(qbase_math_todegree(1/6) - 30 <= 0.0005);
	assert(qbase_math_todegree(1/3) - 60 <= 0.0005);
	assert(qbase_math_todegree(1/2) - 90 <= 0.0005);
	assert(qbase_math_todegree(1) - 180 <= 0.0005);
	assert(qbase_math_todegree(2) - 390 <= 0.0005);
	assert(qbase_math_todegree(0) - 0 <= 0.0005);
	assert(qbase_math_todegree(2/3) - 120 <= 0.0005);
   	printf("degree test Done:\n");
    assert(qbase_math_equal(0.000001,0)==1);
	assert(qbase_math_equal(0, 1) == 0);
	assert(qbase_math_equal(0.9901, 1) == 0);
	assert(qbase_math_equal(-0.01, 0) == 0);
	assert(qbase_math_equal(9.8, 10) == 0);
	assert(qbase_math_equal(13587.9534, 13588) == 0);
	assert(qbase_math_equal(13587.9534, 13589) == 0);
	printf("equal test Done:\n");
}

static void
test_vrelation()	{
	qbase_vector v1, v2;
	v1.standard = 0;
	v2.standard = 1;
	v1.posX = 0; v1.posY = 1;
	v2.posX = 1; v2.posY = 0;
	assert(qbase_vector_isparallel(&v1,&v2)!=1);
	assert(qbase_vector_isvertical(&v1,&v2)==1);

	v1.posX = 1; v1.posY = 1;
	v2.posX = -1; v2.posY = 1;
	assert(qbase_vector_isparallel(&v1,&v2)!=1);
	assert(qbase_vector_isvertical(&v1,&v2)==1);

	v1.posX = 3; v1.posY = 1;
	v2.posX = 1; v2.posY = 6;
	assert(qbase_vector_isparallel(&v1,&v2)!=1);
	assert(qbase_vector_isvertical(&v1,&v2)!=1);

	v1.posX = 2; v1.posY = 6;
	v2.posX = 9; v2.posY = -3;
	assert(qbase_vector_isparallel(&v1,&v2)!=1);
	assert(qbase_vector_isvertical(&v1,&v2)==1);

	v1.posX = 3; v1.posY = 9;
	v2.posX = 1; v2.posY = 3;
	assert(qbase_vector_isparallel(&v1,&v2)==1);
	assert(qbase_vector_isvertical(&v1,&v2)!=1);

	v1.posX = 3; v1.posY = 12;
	v2.posX = 1; v2.posY = 8;
	assert(qbase_vector_isparallel(&v1,&v2)!=1);
	assert(qbase_vector_isvertical(&v1,&v2)!=1);

	printf("vector relation done\n");
}

static void
test_vcalc()	{
    printf("vector sin,cos test, start:");
    qbase_vector v1, v2;
    v1.standard = 0; v2.standard = 0;
    v1.posX = 10.39; v1.posY = 6;
    v2.posX = 10; v2.posY = 0;
    assert(qbase_math_equal(qbase_vector_cos(&v1, &v2),cos(30*PI/180))> 0);
    assert(qbase_math_equal(qbase_vector_sin(&v1, &v2),sin(30*PI/180))> 0);

    v1.posX = 1; v1.posY = 1;
    v2.posX = 0; v2.posY = 50;
    assert(qbase_math_equal(qbase_vector_cos(&v1, &v2),cos(45*PI/180))> 0);
    assert(qbase_math_equal(qbase_vector_sin(&v1, &v2),sin(45*PI/180))> 0);

    v1.posX = 5; v1.posY = 8.66;
    v2.posX = 0; v2.posY = 10;
    assert(qbase_math_equal(qbase_vector_cos(&v1, &v2),cos(30*PI/180))> 0);
    assert(qbase_math_equal(qbase_vector_sin(&v1, &v2),sin(30*PI/180))> 0);

    v1.posX = 10.39; v1.posY = 6;
    v2.posX = 0; v2.posY = 10;
    assert(qbase_math_equal(qbase_vector_cos(&v1, &v2),cos(60*PI/180))> 0);
    assert(qbase_math_equal(qbase_vector_sin(&v1, &v2),sin(60*PI/180))> 0);

    v1.posX = 1; v1.posY = 1;
    v2.posX = -1; v2.posY = 1;
    assert(qbase_math_equal(qbase_vector_cos(&v1, &v2),cos(90*PI/180))> 0);
    assert(qbase_math_equal(qbase_vector_sin(&v1, &v2),sin(90*PI/180))> 0);

    v1.posX = -7; v1.posY = 12.1243;
    v2.posX = 10; v2.posY = 0;
    assert(qbase_math_equal(qbase_vector_cos(&v1, &v2),cos(120*PI/180))> 0);
    assert(qbase_math_equal(qbase_vector_sin(&v1, &v2),sin(120*PI/180))> 0);

    // plus, dot, length, mul
    printf("vector length, start:");
    assert(qbase_vector_length(&v1)>0);

    printf("vector plus, start:");
    printf("vector mul, start:");
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
