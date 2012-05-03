#include <math.h>
#include "basemath.h"

#define PI 3.1415f
#define ES 0.005f
#define MSTD_ROW 0
#define MSTD_COL 1
#define SWAP_AB(A,B)	\
	(A) ^= (B);	\
	(B) ^= (A);	\
	(A) ^= (B)
	

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
	return n-m<=ES;
}

/*	vector functions	*/

inline qbase_vector qbase_vector_plus(const qbase_vector* v1, const qbase_vector* v2)	{
	qbase_vector v;
	v.posX = v1.posX+v2.posX;
	v.posY = v1.posY+v2.posY;
	return v;
}
inline Real qbase_vector_dot(const qbase_vector* v1, const qbase_vector* v2)	{
	return v1.posX*v2.posX + v1.posY*v2.posY;
}
inline Real qbase_vector_cos(const qbase_vector* vec1, const qbase_vector* vec2)	{
	return qbase_vector_dot(vec1,vec2)/(qbase_vector_length(v1)*qbase_vector_length(v2));
}
inline Real qbase_vector_sin(const qbase_vector* vec1, const qbase_vector* vec2)	{
	if(vec1==NULL || vec2==NULL)
		return -2;
	Real cval = qbase_vector_cos(vec1,vec2);
	return 1-cval*cval;
}
inline int qbase_vector_isparallel(const qbase_vector* vec1, const qbase_vector* vec2)	{
	if(vec1==NULL || vec2==NULL)
		return -1;
	return (v1.posX/v2.posX == v1.posY/v2.posY)?1:0;
}
inline int qbase_vector_isvertical(const qbase_vector* vec1, const qbase_vector* vec2)	{
	if(vec1==NULL || vec2==NULL)
		return -1;
	return qbase_vector_dot(vec1,vec2)==0?1:0;
}
inline Real qbase_vector_length(const qbase_vector* vec)	{
	if(vec==NULL)
		return 0;
	return sqrt(vec.posX*vec.posX+vec.posY*vec.posY);
}
inline void qbase_vector_mul(qbase_vector* v, Real k)	{
	if(v==NULL) return NULL;
	v.posX*=k;
	v.posY*=k;
}

/*		matrix functions	*/

inline void qbase_matrix_init(qbase_matrix2 *mtrx, int standard, Real m00, Real m01, Real m10, Real m11)	{
	/*	if mtrx data is not null, release it	*/
	if(mtrx->m!=NULL)	{
		free(mtrx->m);
	}
	mtrx->standard = standard==0?MSTD_ROW:MSTD_COL;		// default is row vector
	mtrx->m = matrix_new();
	mtrx->m[0][0] = m00;
	mtrx->m[0][1] = m01;
	mtrx->m[1][0] = m10;
	mtrx->m[1][1] = m11;
	
	return m;
}
inline void qbase_matrix_trans(qbase_matrix2* mtx)	{
	SWAP_AB(mtx->m[0][1], mtx->m[1][0]);
}
inline qbase_matrix2 qbase_matrix_adjoint(const qbase_matrix2* mtx)	{
	
}
inline void qbase_matrix_reverse(qbase_matrix2* mtx)	{
	
}
inline Real qbase_matrix_det(const qbase_matrix2* mtx)	{
	return mtx->m[0][0]*mtx->[1][1]-mtx->[0][1]*mtx->m[1][0];
}
inline void qbase_matrix_extend(qbase_matrix2* mtx, Real k)	{
	mtx->m[0][0] *= k;
	mtx->m[0][1] *= k;
	mtx->m[1][0] *= k;
	mtx->m[1][1] *= k;
}
inline qbase_matrix2 qbase_matrix_mul(const qbase_matrix2* mtx1, const qbase_matrix2* mtx2)	{
	
}
inline qbase_vector qbase_matrix_vmul(const qbase_vector* vec, const qbase_matrix2* mtx)	{
	
}