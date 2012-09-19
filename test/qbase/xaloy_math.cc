#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <Quab/qbase/qbase.h>
#include <xaloy.h>
using namespace std;
using namespace xaloy;

#define PI 3.1415926f
#define Q_EQUAL(x,y) qbase_math_equal((x), (y))

XALOY_TEST_MODULE(mathBasic)
{
	XALOY_EXPECT(XL_EQUAL, qbase_math_todegree(PI/6), 30.0f);
	XALOY_EXPECT(XL_EQUAL, qbase_math_todegree(PI/3), 60.0f);
	XALOY_EXPECT(XL_EQUAL, qbase_math_todegree(PI/2), 90.0f);
	XALOY_EXPECT(XL_EQUAL, qbase_math_todegree(PI), 180.0f);
	XALOY_EXPECT(XL_EQUAL, qbase_math_todegree(2*PI), 360.0f);
	XALOY_EXPECT(XL_EQUAL, qbase_math_todegree(0) , 0.0f);
	XALOY_EXPECT(XL_EQUAL, qbase_math_todegree(2*PI/3), 120.0f);

    XALOY_EXPECT(XL_EQUAL, qbase_math_equal(0.000001,0), 1);
	XALOY_EXPECT(XL_EQUAL, qbase_math_equal(0, 1),  0);
	XALOY_EXPECT(XL_EQUAL, qbase_math_equal(0.9901, 1), 0);
	XALOY_EXPECT(XL_EQUAL, qbase_math_equal(-0.01, 0), 0);
	XALOY_EXPECT(XL_EQUAL, qbase_math_equal(9.8, 10),  0);
	XALOY_EXPECT(XL_EQUAL, qbase_math_equal(13587.9534, 13588), 0);
	XALOY_EXPECT(XL_EQUAL, qbase_math_equal(13587.9534, 13589), 0);
}

XALOY_TEST_MODULE(mathVector)
{
	qbase_vector v1, v2, v;
	v1.standard = 0;
	v2.standard = 1;
	v1.posX = 0; v1.posY = 1;
	v2.posX = 1; v2.posY = 0;
	XALOY_EXPECT(XL_NOT_EQUAL, qbase_vector_isparallel(&v1,&v2), 1);
	XALOY_EXPECT(XL_EQUAL, qbase_vector_isvertical(&v1,&v2), 1);

	v1.posX = 1; v1.posY = 1;
	v2.posX = -1; v2.posY = 1;
	XALOY_EXPECT(XL_NOT_EQUAL, qbase_vector_isparallel(&v1,&v2), 1);
	XALOY_EXPECT(XL_EQUAL, qbase_vector_isvertical(&v1,&v2), 1);

	v1.posX = 3; v1.posY = 1;
	v2.posX = 1; v2.posY = 6;
	XALOY_EXPECT(XL_NOT_EQUAL, qbase_vector_isparallel(&v1,&v2), 1);
	XALOY_EXPECT(XL_NOT_EQUAL, qbase_vector_isvertical(&v1,&v2), 1);

	v1.posX = 2; v1.posY = 6;
	v2.posX = 9; v2.posY = -3;
	XALOY_EXPECT(XL_NOT_EQUAL, qbase_vector_isparallel(&v1,&v2), 1);
	XALOY_EXPECT(XL_EQUAL, qbase_vector_isvertical(&v1,&v2), 1);

	v1.posX = 3; v1.posY = 9;
	v2.posX = 1; v2.posY = 3;
	XALOY_EXPECT(XL_EQUAL, qbase_vector_isparallel(&v1,&v2), 1);
	XALOY_EXPECT(XL_NOT_EQUAL, qbase_vector_isvertical(&v1,&v2), 1);

	v1.posX = 3; v1.posY = 12;
	v2.posX = 1; v2.posY = 8;
	XALOY_EXPECT(XL_NOT_EQUAL, qbase_vector_isparallel(&v1,&v2), 1);
	XALOY_EXPECT(XL_NOT_EQUAL, qbase_vector_isvertical(&v1,&v2), 1);
	
	/***************  vector's calculate  ****************/	
    v1.standard = 0; v2.standard = 0;
    v1.posX = 10.39; v1.posY = 6;
    v2.posX = 10; v2.posY = 0;
    XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_cos(&v1, &v2),cos(30*PI/180)), 0);
    XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_sin(&v1, &v2),sin(30*PI/180)), 0);

    v1.posX = 1; v1.posY = 1;
    v2.posX = 0; v2.posY = 50;
    XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_cos(&v1, &v2),cos(45*PI/180)), 0);
    XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_sin(&v1, &v2),sin(45*PI/180)), 0);

    v1.posX = 5; v1.posY = 8.66;
    v2.posX = 0; v2.posY = 10;
    XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_cos(&v1, &v2),cos(30*PI/180)), 0);
    XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_sin(&v1, &v2),sin(30*PI/180)), 0);

    v1.posX = 10.39; v1.posY = 6;
    v2.posX = 0; v2.posY = 10;
    XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_cos(&v1, &v2),cos(60*PI/180)), 0);
    XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_sin(&v1, &v2),sin(60*PI/180)), 0);

    v1.posX = 1; v1.posY = 1;
    v2.posX = -1; v2.posY = 1;
    XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_cos(&v1, &v2),cos(90*PI/180)), 0);
    XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_sin(&v1, &v2),sin(90*PI/180)), 0);

    v1.posX = -7; v1.posY = 12.1243;
    v2.posX = 10; v2.posY = 0;
    XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_cos(&v1, &v2),cos(120*PI/180)), 0);
    XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_sin(&v1, &v2),sin(120*PI/180)), 0);

    // plus, dot, length, mul
    v1.posX = 1; v1.posY = 1;
    XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_length(&v1),1.414), 0);
    v1.posX = 1; v1.posY = -1;
    XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_length(&v1),1.414), 0);
    v1.posX = -1; v1.posY = -1;
    XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_length(&v1),1.414), 0);
    v1.posX = 4; v1.posY = 3;
    XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_length(&v1),5), 0);
    v1.posX = 8; v1.posY = 6;
    XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_length(&v1),10), 0);

    v1.posX=10; v1.posY=20;
    v2.posX=5; v2.posY=5;
    v = qbase_vector_plus(&v1, &v2);
    XALOY_EXPECT(XL_EQUAL, (int)(v.posX== v1.posX+v2.posX && v.posY == v1.posY+v2.posY), 1);

    v1.posX=10; v1.posY=-20;
    v2.posX=5; v2.posY=5;
    v = qbase_vector_plus(&v1, &v2);
    XALOY_EXPECT(XL_EQUAL, (int)(v.posX== v1.posX+v2.posX && v.posY == v1.posY+v2.posY), 1);

    v1.posX=0.31; v1.posY=20;
    v2.posX=5.39; v2.posY=5.092;
    v = qbase_vector_plus(&v1, &v2);
    XALOY_EXPECT(XL_EQUAL, (int)(
           qbase_math_equal(v.posX, v1.posX+v2.posX)>0 &&
           qbase_math_equal(v.posY, v1.posY+v2.posY)>0
    ), 1);

    v1.posX=-10.3; v1.posY=2.0;
    v2.posX=5; v2.posY=5;
    v = qbase_vector_plus(&v1, &v2);
    XALOY_EXPECT(XL_EQUAL, (int)(v.posX== v1.posX+v2.posX && v.posY == v1.posY+v2.posY), 1);

	v1.posX = 10; v1.posY = 5;
	v2.posX = 0.3; v2.posY = 12.1;
	XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_dot(&v1, &v2), 63.5), 0);
	v1.posX = -5; v1.posY = 1;
	v2.posX = 0; v2.posY = 7;
	XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_dot(&v1, &v2), 7.0), 0);
	v1.posX = -1; v1.posY = 2;
	v2.posX = -0.3; v2.posY = -4;
	XALOY_EXPECT(XL_GREATER, qbase_math_equal(qbase_vector_dot(&v1, &v2), -7.7), 0);

	v1.posX = 10; v1.posY = 5;
	v = v1;
	qbase_vector_mul(&v, 0.5);
	XALOY_EXPECT(XL_GREATER, qbase_math_equal(v.posX, 0.5*v1.posX)>0 && qbase_math_equal(v.posY, 0.5*v1.posY), 0);
	v1.posX = 0; v1.posY = 0;
	v = v1;
	qbase_vector_mul(&v, 3);
	XALOY_EXPECT(XL_GREATER, qbase_math_equal(v.posX, 3*v1.posX)>0 && qbase_math_equal(v.posY, 3*v1.posY), 0);
	v1.posX = 120; v1.posY = 5.17;
	v = v1;
	qbase_vector_mul(&v, 1.75);
	XALOY_EXPECT(XL_GREATER, qbase_math_equal(v.posX, 1.75*v1.posX)>0 && qbase_math_equal(v.posY, 1.75*v1.posY), 0);
	v1.posX = 13.902; v1.posY = -5.07;
	v = v1;
	qbase_vector_mul(&v, -0.5);
	XALOY_EXPECT(XL_GREATER, qbase_math_equal(v.posX, -0.5*v1.posX)>0 && qbase_math_equal(v.posY, -0.5*v1.posY), 0);
}

XALOY_TEST_MODULE(mathMatrix)
{
	// repeat initalize the matrix for testing the wild pointer
	qbase_matrix2 m1, mt, ma;
	Real det;
	qbase_vector v;
	//	check zero and normal construction
	qbase_matrix_zero(&m1, V_ROW);
	XALOY_EXPECT(XL_EQUAL, (int)(
		Q_EQUAL(m1.m[0][0], 0.0)>0 && Q_EQUAL(m1.m[0][1], 0.0)>0 &&
		Q_EQUAL(m1.m[1][0], 0.0)>0 && Q_EQUAL(m1.m[1][1], 0.0)>0
	), 1);

	qbase_matrix_init(&m1, V_ROW, 1, 2, 3, 4);
	XALOY_EXPECT(XL_EQUAL, (int)(
		Q_EQUAL(m1.m[0][0], 1.0)>0 && Q_EQUAL(m1.m[0][1], 2.0)>0 &&
		Q_EQUAL(m1.m[1][0], 3.0)>0 && Q_EQUAL(m1.m[1][1], 4.0)>0
	), 1);

	// trans test, use the same matrix of init test
	qbase_matrix_trans(&m1);
	XALOY_EXPECT(XL_EQUAL, (int)(
		Q_EQUAL(m1.m[0][0], 1.0)>0 && Q_EQUAL(m1.m[0][1], 3.0)>0 &&
		Q_EQUAL(m1.m[1][0], 2.0)>0 && Q_EQUAL(m1.m[1][1], 4.0)>0
	), 1);

	// extend of matrix, use the same data of trans
	qbase_matrix_extend(&m1, 0.5);
	XALOY_EXPECT(XL_EQUAL, (int)(
		Q_EQUAL(m1.m[0][0], 0.5)>0 && Q_EQUAL(m1.m[0][1], 1.5)>0 &&
		Q_EQUAL(m1.m[1][0], 1.0)>0 && Q_EQUAL(m1.m[1][1], 2.0)>0
	), 1);
	qbase_matrix_extend(&m1, -2);
	XALOY_EXPECT(XL_EQUAL, (int)(
		Q_EQUAL(m1.m[0][0], -1.0)>0 && Q_EQUAL(m1.m[0][1], -3.0)>0 &&
		Q_EQUAL(m1.m[1][0], -2.0)>0 && Q_EQUAL(m1.m[1][1], -4.0)>0
	), 1);
	qbase_matrix_extend(&m1, 0);
	XALOY_EXPECT(XL_EQUAL, (int)(
		Q_EQUAL(m1.m[0][0], 0.0)>0 && Q_EQUAL(m1.m[0][1], 0.0)>0 &&
		Q_EQUAL(m1.m[1][0], 0.0)>0 && Q_EQUAL(m1.m[1][1], 0.0)>0
	), 1);
	
	/**********  matrix's calculate  ********/
	// adjoint matrix test	
    qbase_matrix_init(&mt, V_ROW, 1,4,3,5);
    ma = qbase_matrix_adjoint(&mt);
    XALOY_EXPECT(XL_EQUAL, (int)(
        Q_EQUAL(ma.m[0][0], mt.m[1][1])>0 && Q_EQUAL(ma.m[0][1], -mt.m[0][1])>0 &&
		Q_EQUAL(ma.m[1][0], -mt.m[1][0])>0 && Q_EQUAL(ma.m[1][1], mt.m[0][0])>0
    ), 1);
	qbase_matrix_init(&mt, V_ROW, 4,-2.5,3.01,-7.312);
    ma = qbase_matrix_adjoint(&mt);
    XALOY_EXPECT(XL_EQUAL, (int)(
        Q_EQUAL(ma.m[0][0], mt.m[1][1])>0 && Q_EQUAL(ma.m[0][1], -mt.m[0][1])>0 &&
		Q_EQUAL(ma.m[1][0], -mt.m[1][0])>0 && Q_EQUAL(ma.m[1][1], mt.m[0][0])>0
    ), 1);

	// matrix det value test
	qbase_matrix_init(&mt, V_ROW, 1, 1, 1, 1);
	det = qbase_matrix_det(&mt);
	XALOY_EXPECT(XL_GREATER, Q_EQUAL(det, 0.0), 0);
	qbase_matrix_init(&mt, V_ROW, 5, 2.5, 4, 4);
	det = qbase_matrix_det(&mt);
	XALOY_EXPECT(XL_GREATER, Q_EQUAL(det, 10.0), 0);
	qbase_matrix_init(&mt, V_ROW, 1, 2, -3, 4);
	det = qbase_matrix_det(&mt);
	XALOY_EXPECT(XL_GREATER, Q_EQUAL(det, 10.0), 0);
	qbase_matrix_init(&mt, V_ROW, -1, 1, 19, -31);
	det = qbase_matrix_det(&mt);
	XALOY_EXPECT(XL_GREATER, Q_EQUAL(det, 12.0), 0);

	// matrix mul test
	qbase_matrix_init(&mt, V_ROW, -3, 0, 5, 0.5);
	qbase_matrix_init(&ma, V_ROW, -7, 2, 4, 6);
	ma = qbase_matrix_mul(&mt, &ma);
	XALOY_EXPECT(XL_EQUAL, (int)(
		Q_EQUAL(ma.m[0][0], 21)>0 && Q_EQUAL(ma.m[0][1], -6)>0 &&
		Q_EQUAL(ma.m[1][0], -33)>0 && Q_EQUAL(ma.m[1][1], 13)>0
	), 1);

	// matrix vmul test
	v.posX = 0;
	v.posY = 1;
	v.standard = V_ROW;
	qbase_matrix_init(&mt, V_ROW, 2, 1, -1, 2);
	v = qbase_matrix_vmul(&v, &mt);
	XALOY_EXPECT(XL_EQUAL, (int)(Q_EQUAL(v.posX, -1)>0 && Q_EQUAL(v.posY, 2)>0), 1);
	v.posX = 1;
	v.posY = 0;
	v = qbase_matrix_vmul(&v, &mt);
	XALOY_EXPECT(XL_EQUAL, (int)(Q_EQUAL(v.posX, 2)>0 && Q_EQUAL(v.posY, 1)>0), 1);

	// matrix inverse test
	qbase_matrix_inverse(&mt);
	XALOY_EXPECT(XL_EQUAL, (int)(
		Q_EQUAL(mt.m[0][0], 0.4)>0 && Q_EQUAL(mt.m[0][1], -0.2)>0 &&
		Q_EQUAL(mt.m[1][0], 0.2)>0 && Q_EQUAL(mt.m[1][1], 0.4)>0
	), 1);
	qbase_matrix_init(&mt, V_ROW, -7, 2, 4, 6);
	qbase_matrix_inverse(&mt);
	XALOY_EXPECT(XL_EQUAL, (int)(
		Q_EQUAL(mt.m[0][0], 0.12f)>0 && Q_EQUAL(mt.m[0][1], -0.04f)>0 &&
		Q_EQUAL(mt.m[1][0], -0.08f)>0 && Q_EQUAL(mt.m[1][1], -0.14f)>0
	), 1);
}

int main()
{
	XaloyTester::SetAllowPrec(0.0005);
	return G_XALOY.execute(true);
}
