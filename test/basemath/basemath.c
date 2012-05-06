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

/*******************   static func	*********************/
static Real**
matrix_new()	{
	Real **m = NULL;
	m = malloc(2*sizeof(Real*)+4*sizeof(Real));
	*(m) = (Real*)(m+2);
	*(m+1) = (Real*)(m+4);
	*(Real*)(m+2) = 0.0f;
	*(Real*)(m+3) = 0.0f;
	*(Real*)(m+4) = 0.0f;
	*(Real*)(m+5) = 0.0f;
	return m;
}

/*******************	implement	*********************/

/*	basic functions	*/
inline Real qbase_math_todegree(Real rad)	{
	return 180*rad/PI;
}

inline int qbase_math_equal(Real n, Real m)	{
	Real diff = n-m;
	return (diff<0)?(diff>=(-ES)):(diff<=ES);
}

/*	vector functions	*/

inline qbase_vector qbase_vector_plus(const qbase_vector* v1, const qbase_vector* v2)	{
	qbase_vector v;
	v.posX = v1->posX+v2->posX;
	v.posY = v1->posY+v2->posY;
	return v;
}
inline Real qbase_vector_dot(const qbase_vector* v1, const qbase_vector* v2)	{
	return v1->posX*v2->posX + v1->posY*v2->posY;
}
inline Real qbase_vector_cos(const qbase_vector* vec1, const qbase_vector* vec2)	{
	return qbase_vector_dot(vec1,vec2)/(qbase_vector_length(vec1)*qbase_vector_length(vec2));
}
inline Real qbase_vector_sin(const qbase_vector* vec1, const qbase_vector* vec2)	{
	if(vec1==NULL || vec2==NULL)
		return -2;
	Real cval = qbase_vector_cos(vec1,vec2);
	return sqrt(1-cval*cval);
}
inline int qbase_vector_isparallel(const qbase_vector* vec1, const qbase_vector* vec2)	{
	if(vec1==NULL || vec2==NULL)
		return -1;
	return qbase_math_equal(vec1->posX*vec2->posY, vec1->posY*vec2->posX)>0?1:0;
}
inline int qbase_vector_isvertical(const qbase_vector* vec1, const qbase_vector* vec2)	{
	if(vec1==NULL || vec2==NULL)
		return -1;
	return qbase_math_equal(qbase_vector_dot(vec1,vec2), 0)>0?1:0;
}
inline Real qbase_vector_length(const qbase_vector* vec)	{
	if(vec==NULL)
		return 0;
	return sqrt(vec->posX*vec->posX+vec->posY*vec->posY);
}
inline int qbase_vector_mul(qbase_vector* v, Real k)	{
	if(v==NULL) return 0;
	v->posX*=k;
	v->posY*=k;
	return 1;
}

/*		matrix functions	*/
inline void qbase_matrix_free(qbase_matrix2* m) {
    if(m->m!= NULL) {
        free(m->m);
        m->m = NULL;
    }
}

inline int qbase_matrix_zero(qbase_matrix2* m, int standard)	{
	return qbase_matrix_init(m, standard, 0, 0, 0, 0);
}

inline int qbase_matrix_init(qbase_matrix2* mtrx, int standard, Real m00, Real m01, Real m10, Real m11)	{
	/*	if mtrx data is not null, release it	*/
	qbase_matrix_free(mtrx);

	mtrx->standard = standard==0?ROW_BASE:COL_BASE;		//  [tablename] is row vector
	mtrx->m = matrix_new();
	if(mtrx->m == NULL)
        return 0;
	mtrx->m[0][0] = m00;
	mtrx->m[0][1] = m01;
	mtrx->m[1][0] = m10;
	mtrx->m[1][1] = m11;
	return 1;
}
inline void qbase_matrix_trans(qbase_matrix2* mtx)	{
	SWAP_AB(mtx->m[0][1], mtx->m[1][0]);
}
inline qbase_matrix2 qbase_matrix_adjoint(const qbase_matrix2* mtx)	{
	qbase_matrix2 m;
	m.m = NULL;
	//   2x2 matrix 	-->		adjoint matrix
	//    a		b				 d		-c
	//	  c		d				-b		 a
	qbase_matrix_init(&m, ROW_BASE, mtx->m[1][1], -mtx->m[1][0], -mtx->m[0][1], mtx->m[0][0]);
	return m;
}
inline void qbase_matrix_inverse(qbase_matrix2* mtx)	{
	// this function is need to optimize
	qbase_matrix2 m = qbase_matrix_adjoint(mtx);
	Real det = qbase_matrix_det(mtx);
	mtx->m[0][0] = m.m[0][0];
	mtx->m[0][1] = m.m[0][1];
	mtx->m[1][0] = m.m[1][0];
	mtx->m[1][1] = m.m[1][1];

	qbase_matrix_extend(mtx, det);
}
inline Real qbase_matrix_det(const qbase_matrix2* mtx)	{
	return mtx->m[0][0]*mtx->m[1][1]-mtx->m[0][1]*mtx->m[1][0];
}
inline void qbase_matrix_extend(qbase_matrix2* mtx, Real k)	{
	mtx->m[0][0] *= k;
	mtx->m[0][1] *= k;
	mtx->m[1][0] *= k;
	mtx->m[1][1] *= k;
}
inline qbase_matrix2 qbase_matrix_mul(const qbase_matrix2* mtx1, const qbase_matrix2* mtx2)	{
	// need to optimize at next version
	// the multiplication of 2x2 matrix:
	// | a1		b1 |	*	| a2	b2 | =	| a1[a2 b2] + b1[c2 d2] |
	// | c1		d1 |	 	| c2	d2 |	| c1[a2 b2] + d1[c2 d2] |
	qbase_matrix2 m;
	qbase_matrix_init(&m, ROW_BASE,
		mtx1->m[0][0]*mtx2->m[0][0]+mtx1->m[0][1]*mtx2->m[1][0], mtx1->m[0][0]*mtx2->m[0][1]+mtx1->m[0][1]*mtx2->m[1][1],
		mtx1->m[1][0]*mtx2->m[0][0]+mtx1->m[1][1]*mtx2->m[1][0], mtx1->m[1][0]*mtx2->m[0][1]+mtx1->m[1][1]*mtx2->m[1][1]
	);
	return m;
}
inline qbase_vector qbase_matrix_vmul(const qbase_vector* vec, const qbase_matrix2* mtx)	{
	qbase_vector v;
	v.standard = vec->standard;
	if(vec->standard == 0)
	{
		v.posX = vec->posX*mtx->m[0][0] + vec->posY*mtx->m[1][0];
		v.posY = vec->posX*mtx->m[0][1] + vec->posY*mtx->m[1][1];
	}
	else
	{
		v.posX = vec->posX*mtx->m[0][0] + vec->posY*mtx->m[0][1];
		v.posY = vec->posX*mtx->m[1][0] + vec->posY*mtx->m[1][1];
	}
	return v;
}
