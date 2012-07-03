#ifndef QBASE_MATH_H
#define QBASE_MATH_H

#define V_ROW 0
#define	V_COL 1

#ifdef QBASE_OS_64BIT
typedef double Real;
#else
typedef float Real;
#endif

typedef Real** matrix_ptr;
typedef struct	qbase_vector	{
	Real posX;
	Real posY;
	int standard;
} qbase_vector;

typedef struct qbase_matrix2	{
	Real m[2][2];
	int standard;
} qbase_matrix2;

/*	basic	*/
Real qbase_math_todegree(Real rad);
int qbase_math_equal(Real n, Real m);

/*	vector	*/
qbase_vector qbase_vector_plus(const qbase_vector* v1, const qbase_vector* v2);
Real qbase_vector_dot(const qbase_vector* v1, const qbase_vector* v2);
Real qbase_vector_cos(const qbase_vector* vec1, const qbase_vector* vec2);
Real qbase_vector_sin(const qbase_vector* vec1, const qbase_vector* vec2);
int qbase_vector_isparallel(const qbase_vector* vec1, const qbase_vector* vec2);
int qbase_vector_isvertical(const qbase_vector* vec1, const qbase_vector* vec2);
Real qbase_vector_length(const qbase_vector* vec);
int qbase_vector_mul(qbase_vector* v, Real k);

/*	matrix	*/
int qbase_matrix_zero(qbase_matrix2* m, int standard);
int qbase_matrix_init(qbase_matrix2* m, int standard, Real m00, Real m01, Real m10, Real m11);
qbase_matrix2 qbase_matrix_plus(const qbase_matrix2 *m1, const qbase_matrix2 *m2);
void qbase_matrix_trans(qbase_matrix2* mtx);
qbase_matrix2 qbase_matrix_adjoint(const qbase_matrix2* mtx);
void qbase_matrix_inverse(qbase_matrix2* mtx);
Real qbase_matrix_det(const qbase_matrix2* mtx);
void qbase_matrix_extend(qbase_matrix2* mtx, Real k);
qbase_matrix2 qbase_matrix_mul(const qbase_matrix2* mtx1, const qbase_matrix2* mtx2);
qbase_vector qbase_matrix_vmul(const qbase_vector* vec, const qbase_matrix2* mtx);

#endif
