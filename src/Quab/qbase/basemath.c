#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include "basemath.h"

#define PI 3.1415926f
#define ES 0.005f
#define ROW_BASE 0
#define COL_BASE 1
#define SWAP_AB(A,B)	\
	(A) += (B);	\
	(B) = (A) - (B);	\
	(A) -= (B)

	
/*     helper static functions     */
static void mtx_adjoint(const qbase_matrix2 *m)	{
	//   2x2 matrix 	-->		adjoint matrix(Aij=Mji)
	//    a		b				 d		-b
	//	  c		d				-c		 a
	if(m == NULL)
		return;
	SWAP_AB(m->m[0][0], m->[1][1]);
	SWAP_AB(m->m[0][1], m->[1][0]);
	m->[0][1] *= -1;
	m->[1][0] *= -1;
}

/*******************	implement	*********************/

/*	basic functions	*/
Real qbase_math_todegree(Real rad)	{
	return 180*rad/PI;
}

int qbase_math_equal(Real n, Real m)	{
	Real diff = n-m;
	return (diff<0)?(diff>=(-ES)):(diff<=ES);
}

int qbase_math_equalV(const qbase_vector* v1, const qbase_vector* v2)	{
	Real diff_x, diff_y;
	if(v1 == NULL || v2 == NULL)
		return 0;
	diff_x = v1->posX - v2->posX, diff_y = v1->posY - v2->posY;
	if(fabs(diff_x) < ES && fabs(diff_y) < ES)
		return 1;
	else return 0;
}

int qbase_math_equalM(const qbase_matrix2* m1, const qbase_matrix2* m2)	{
	int i;
	if(m1 == NULL || m2 == NULL)
		return 0;
	if(m1->standard != m2->standard)
		return 0;
	else	{
		for(i = 0; i < 4; i++)	{
			if(qbase_math_equal(m1->m[i/2][i%2], m2->m[i/2][i%2]) == 0)
				return 0;
		}
		return 1;
	}
}

/*	vector functions	*/
qbase_vector qbase_vector_copy(const qbase_vector *v)	{
	qbase_vector vec;	
	vec.posX = v->posX;
	vec.posY = v->posY;
	vec.standard = v->standard;
	return vec;
}

void qbase_vector_selfplus(qbase_vector* v1, const qbase_vector* v2)	{
	v1->posX += v2->posX;
	v1->posY += v2->posY;
}

qbase_vector qbase_vector_plus(const qbase_vector* v1, const qbase_vector* v2)	{
	qbase_vector v = qbase_vector_copy(v1);
	qbase_vector_selfplus(&v, v2)
	return v;
}
Real qbase_vector_dot(const qbase_vector* v1, const qbase_vector* v2)	{
	return v1->posX*v2->posX + v1->posY*v2->posY;
}
Real qbase_vector_cos(const qbase_vector* vec1, const qbase_vector* vec2)	{
	return qbase_vector_dot(vec1,vec2)/(qbase_vector_length(vec1)*qbase_vector_length(vec2));
}
Real qbase_vector_sin(const qbase_vector* vec1, const qbase_vector* vec2)	{
	Real cval;
	if(vec1==NULL || vec2==NULL)
		return -2;
	cval = qbase_vector_cos(vec1,vec2);
	return sqrt(1-cval*cval);
}
int qbase_vector_isparallel(const qbase_vector* vec1, const qbase_vector* vec2)	{
	if(vec1==NULL || vec2==NULL)
		return -1;
	return qbase_math_equal(vec1->posX*vec2->posY, vec1->posY*vec2->posX)>0?1:0;
}
int qbase_vector_isvertical(const qbase_vector* vec1, const qbase_vector* vec2)	{
	if(vec1==NULL || vec2==NULL)
		return -1;
	return qbase_math_equal(qbase_vector_dot(vec1,vec2), 0)>0?1:0;
}
Real qbase_vector_length(const qbase_vector* vec)	{
	if(vec==NULL)
		return 0;
	return sqrt(vec->posX*vec->posX+vec->posY*vec->posY);
}
int qbase_vector_mul(qbase_vector* v, Real k)	{
	if(v==NULL) return 0;
	v->posX*=k;
	v->posY*=k;
	return 1;
}

int qbase_matrix_zero(qbase_matrix2* m, int standard)	{
	return qbase_matrix_init(m, standard, 0, 0, 0, 0);
}

int qbase_matrix_init(qbase_matrix2* mtrx, int standard, Real m00, Real m01, Real m10, Real m11)	{
	mtrx->standard = standard==0?ROW_BASE:COL_BASE;		//  [tablename] is row vector
	mtrx->m[0][0] = m00;
	mtrx->m[0][1] = m01;
	mtrx->m[1][0] = m10;
	mtrx->m[1][1] = m11;
	return 1;
}

qbase_matrix2 qbase_matrix_copy(const qbase_matrix2 *mtx)	{
	qbase_matrix2 m;
	qbase_matrix_init(&m, ROW_BASE, mtx->m[0][0], mtx->m[0][1], mtx->m[1][0], mtx->m[1][1]);
	return m;
}

void qbase_matrix_selfplus(qbase_matrix2 *m1, const qbase_matrix2 *m2)	{
	if(m1 == NULL || m2 == NULL)
		return;
	m1->m[0][0] += m2->m[0][0];
	m1->m[0][1] += m2->m[0][1];
	m1->m[1][0] += m2->m[1][0];
	m1->m[1][1] += m2->m[1][1];
}

qbase_matrix2 qbase_matrix_plus(const qbase_matrix2 *m1, const qbase_matrix2 *m2)	{
	qbase_matrix2 m = qbase_matrix_copy(m1);
	qbase_matrix_selfplus(&m, m2);
	return m;
}
void qbase_matrix_trans(qbase_matrix2* mtx)	{
	SWAP_AB(mtx->m[0][1], mtx->m[1][0]);
}
qbase_matrix2 qbase_matrix_adjoint(const qbase_matrix2* mtx)	{
	qbase_matrix2 m = qbase_matrix_copy(mtx);
	mtx_adjoint(m);
	return m;
}
void qbase_matrix_inverse(qbase_matrix2* mtx)	{
	// this function is need to optimize
	Real det = qbase_matrix_det(mtx);
	mtx_adjoint(mtx);
	mtx->m[0][0] = m.m[0][0];
	mtx->m[0][1] = m.m[0][1];
	mtx->m[1][0] = m.m[1][0];
	mtx->m[1][1] = m.m[1][1];
	qbase_matrix_extend(mtx, fabs(1.0f/det));
}
Real qbase_matrix_det(const qbase_matrix2* mtx)	{
	return mtx->m[0][0]*mtx->m[1][1]-mtx->m[0][1]*mtx->m[1][0];
}
void qbase_matrix_extend(qbase_matrix2* mtx, Real k)	{
	mtx->m[0][0] *= k;
	mtx->m[0][1] *= k;
	mtx->m[1][0] *= k;
	mtx->m[1][1] *= k;
}

void qbase_matrix_selfmul(qbase_matrix2* mtx1, const qbase_matrix2* mtx2)	{
	if(mtx1 == NULL || mtx2 == NULL)
		return;
		
	// need to optimize at next version
	// the multiplication of 2x2 matrix:
	// | a1		b1 |	*	| a2	b2 | =	| a1[a2 b2] + b1[c2 d2] |
	// | c1		d1 |	 	| c2	d2 |	| c1[a2 b2] + d1[c2 d2] |
	mtx1->m[0][0] = mtx1->m[0][0]*mtx2->m[0][0]+mtx1->m[0][1]*mtx2->m[1][0];
	mtx1->m[0][1] = mtx1->m[0][0]*mtx2->m[0][1]+mtx1->m[0][1]*mtx2->m[1][1];
	mtx1->m[1][0] = mtx1->m[1][0]*mtx2->m[0][0]+mtx1->m[1][1]*mtx2->m[1][0];
	mtx1->m[1][1] = mtx1->m[1][0]*mtx2->m[0][1]+mtx1->m[1][1]*mtx2->m[1][1];
}

qbase_matrix2 qbase_matrix_mul(const qbase_matrix2* mtx1, const qbase_matrix2* mtx2)	{	
	qbase_matrix2 m = qbase_matrix_copy(mtx1);
	qbase_matrix_selfmul(&m, mtx2);
	return m;
}

qbase_vector qbase_matrix_selfvmul(qbase_vector* vec, const qbase_matrix2* mtx)	{
	if(vec->standard == 0)
	{
		vec->posX = vec->posX*mtx->m[0][0] + vec->posY*mtx->m[1][0];
		vec->posY = vec->posX*mtx->m[0][1] + vec->posY*mtx->m[1][1];
	}
	else
	{
		vec->posX = vec->posX*mtx->m[0][0] + vec->posY*mtx->m[0][1];
		vec->posY = vec->posX*mtx->m[1][0] + vec->posY*mtx->m[1][1];
	}
}

qbase_vector qbase_matrix_vmul(const qbase_vector* vec, const qbase_matrix2* mtx)	{
	qbase_vector v = qbase_vector_copy(vec);
	qbase_matrix_selfvmul(&v, mtx);
	return v;
}
