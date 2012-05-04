#ifndef QBASE_MATH_H
#define QBASE_MATH_H

typedef float Real;
typedef Real** matrix_ptr;
typedef struct	qbase_vector	{
	Real posX;
	Real posY;
	int standard;
} qbase_vector;

typedef struct qbase_matrix2	{
	matrix_ptr m;
	int standard;
} qbase_matrix2;

/*	basic	*/
inline Real qbase_math_todegree(Real rad);
inline int qbase_math_equal(Real n, Real m);

/*	vector	*/
inline qbase_vector qbase_vector_plus(const qbase_vector* v1, const qbase_vector* v2);
inline Real qbase_vector_dot(const qbase_vector* v1, const qbase_vector* v2);
inline Real qbase_vector_cos(const qbase_vector* vec1, const qbase_vector* vec2);
inline Real qbase_vector_sin(const qbase_vector* vec1, const qbase_vector* vec2);
inline int qbase_vector_isparallel(const qbase_vector* vec1, const qbase_vector* vec2);
inline int qbase_vector_isvertical(const qbase_vector* vec1, const qbase_vector* vec2);
inline Real qbase_vector_length(const qbase_vector* vec);
inline void qbase_vector_mul(qbase_vector* v, Real k);

/*	matrix	*/
inline void qbase_matrix_zero(qbase_matrix2 *m, int standard);
inline void qbase_matrix_init(qbase_matrix2 *m, int standard, Real m00, Real m01, Real m10, Real m11);
inline void qbase_matrix_trans(qbase_matrix2* mtx);
inline qbase_matrix2 qbase_matrix_adjoint(const qbase_matrix2* mtx);
inline void qbase_matrix_inverse(qbase_matrix2* mtx);
inline Real qbase_matrix_det(const qbase_matrix2* mtx);
inline void qbase_matrix_extend(qbase_matrix2* mtx, Real k);
inline qbase_matrix2 qbase_matrix_mul(const qbase_matrix2* mtx1, const qbase_matrix2* mtx2);
inline qbase_vector qbase_matrix_vmul(const qbase_vector* vec, const qbase_matrix2* mtx);

#endif
