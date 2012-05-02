#include <math.h>
#include "basemath.h"

#define PI 3.1415f
#define ES 0.005f

/*******************   static func	*********************/
static qbase_matrix2 *
matrix_new()	{
	qbase_matrix2 * mtrx = malloc(sizeof(qbase_matrix2));
	mtrx->m = malloc(2*sizeof(Real*)+4*sizeof(float));
	*(mtrx->m) = (Real*)(mtrx->m+2);
	*(mtrx->m+1) = (Real*)(mtrx->m+4);
	*(Real*)(mtrx->m+2) = 0.0f;
	*(Real*)(mtrx->m+3) = 0.0f;
	*(Real*)(mtrx->m+4) = 0.0f;
	*(Real*)(mtrx->m+5) = 0.0f;
	return mtrx;
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
	return v1.posX/v2.posX == v1.posY/v2.posY;
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

inline void qbase_matrix_init(qbase_matrix2* m, Real m00, Real m01, Real m10, Real m11)	{
	/*	if m is not null, release it	*/
	if(m!=NULL)	{
		free(m->m);
		free(m);
	}
	m = matrix_new();
	m->m[0][0] = m00;
	m->m[0][1] = m01;
	m->m[1][0] = m10;
	m->m[1][1] = m11;
}
inline void qbase_matrix_trans(qbase_matrix2* mtx)	{
	
}
inline qbase_matrix2 qbase_matrix_adjoint(const qbase_matrix2* mtx)	{
	
}
inline void qbase_matrix_reverse(qbase_matrix2* mtx)	{
	
}
inline Real qbase_matrix_det(const qbase_matrix2* mtx)	{
	
}
inline void qbase_matrix_extend(qbase_matrix2* mtx, Real k)	{
	
}
inline qbase_matrix2 qbase_matrix_mul(const qbase_matrix2* mtx1, const qbase_matrix2* mtx2)	{
	
}
inline qbase_vector qbase_matrix_vmul(const qbase_vector* vec, const qbase_matrix2* mtx)	{
	
}