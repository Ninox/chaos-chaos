#ifndef QBASE_MATH_H
#define QBASE_MATH_H

#define V_ROW 0
#define	V_COL 1

#ifdef QUAB_USE_DOUBLE
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


/** \brief transfer RADIAN to DEGREE
 *
 * \param a radian value, Real
 * \return a degree value
 *
 */
Real qbase_math_todegree(Real rad);

/** \brief check the values in parameters is equal (it means that n-m < epsilon)
 *
 * \param real value_1, Real
 * \param real value_2, Real
 * \return return 1 when the parameters is equal, or return 0;
 *
 */
int qbase_math_equal(Real n, Real m);

/** \brief check the values in parameters is equal (v1.x == v2.x && v1.y == v2.y)
 *
 * \param qbase_vector value_1, qbase_vector
 * \param qbase_vector value_2, qbase_vector
 * \return return 1 when the parameters is equal, or return 0;
 *
 */
int qbase_math_equalV(const qbase_vector* v1, const qbase_vector* v2);

/** \brief check the values in parameters is equal (matrix value and standard)
 *
 * \param qbase_matrix2 value_1, qbase_matrix2
 * \param qbase_matrix2 value_2, qbase_matrix2
 * \return return 1 when the parameters is equal, or return 0;
 *
 */
int qbase_math_equalM(const qbase_matrix2* m1, const qbase_matrix2* m2);

/*	vector	*/

/** \brief get a copy vector
 *
 * \param the vector, const qbase_vector*
 * \return a copy of vector
 *
 */
qbase_vector qbase_vector_copy(const qbase_vector *v);

/** \brief vector's "+=" operation
 *
 * \param vector_1 qbase_vector*
 * \param vector_2 const qbase_vector*
 * \return just modified the vector, no return
 *
 */
void qbase_vector_selfplus(qbase_vector* v1, const qbase_vector* v2);

/** \brief vector's "+" operation
 *
 * \param vector_1 const qbase_vector*
 * \param vector_2 const qbase_vector*
 * \return a vector stand for the result of this operation
 *
 */
qbase_vector qbase_vector_plus(const qbase_vector* v1, const qbase_vector* v2);


/** \brief the dot product between two vectors
 *
 * \param vector_1 const qbase_vector*
 * \param vector_2 const qbase_vector*
 * \return a real value stand for the result of this operation
 *
 */
Real qbase_vector_dot(const qbase_vector* v1, const qbase_vector* v2);


/** \brief get cos value of two vectors
 *
 * \param vec1 const qbase_vector*
 * \param vec2 const qbase_vector*
 * \return the cos value
 *
 */
Real qbase_vector_cos(const qbase_vector* vec1, const qbase_vector* vec2);


/** \brief get sin value of this two vectors
 *
 * \param vec1 const qbase_vector*
 * \param vec2 const qbase_vector*
 * \return sin value
 *
 */
Real qbase_vector_sin(const qbase_vector* vec1, const qbase_vector* vec2);


/** \brief check the vectors in parameters is parallel
 *
 * \param vec1 const qbase_vector*
 * \param vec2 const qbase_vector*
 * \return 1 = true, 0 = false
 *
 */
int qbase_vector_isparallel(const qbase_vector* vec1, const qbase_vector* vec2);


/** \brief check the vectors in parameters is vertical
 *
 * \param vec1 const qbase_vector*
 * \param vec2 const qbase_vector*
 * \return 1 = true, 0 = false
 *
 */
int qbase_vector_isvertical(const qbase_vector* vec1, const qbase_vector* vec2);


/** \brief get vector's lenth
 *
 * \param the vector, const qbase_vector*
 * \return length of the vector
 *
 */
Real qbase_vector_length(const qbase_vector* vec);


/** \brief the product of the vector and scalar
 *
 * \param v qbase_vector*
 * \param k Real
 * \return int
 *
 */
int qbase_vector_mul(qbase_vector* v, Real k);

/*	matrix	*/

/** \brief initalize the zero matrix
 *
 * \param matrix, qbase_matrix2*
 * \param standard of the matrix(row or column) int
 * \return int
 *
 */
int qbase_matrix_zero(qbase_matrix2* m, int standard);

/** \brief initalize the matrix by specified value
 *
 * \param matrix, qbase_matrix2*
 * \param standard of the matrix(row or column), int
 * \param m00 Real
 * \param m01 Real
 * \param m10 Real
 * \param m11 Real
 * \return int
 *
 */
int qbase_matrix_init(qbase_matrix2* m, int standard, Real m00, Real m01, Real m10, Real m11);

/** \brief copy a matrix
 *
 * \param source matrix, const qbase_matrix2*
 * \return return a copy
 *
 */
qbase_matrix2 qbase_matrix_copy(const qbase_matrix2 *mtx);

/** \brief the additive of matrix, it means "m1+=m2"
 *
 * \param m1 qbase_matrix2*
 * \param m2 const qbase_matrix2*
 * \return no return
 *
 */
void qbase_matrix_selfplus(qbase_matrix2 *m1, const qbase_matrix2 *m2);

/** \brief the additive of matrix
 *
 * \param m1 const qbase_matrix2*
 * \param m2 const qbase_matrix2*
 * \return return a new matrix as the additive result
 *
 */
qbase_matrix2 qbase_matrix_plus(const qbase_matrix2 *m1, const qbase_matrix2 *m2);


/** \brief transpose a matrix
 *
 * \param mtx qbase_matrix2*
 * \return void
 *
 */
void qbase_matrix_trans(qbase_matrix2* mtx);

/** \brief get a adjoint matrix
 *
 * \param source matrix, const qbase_matrix2*
 * \return the parameter's adjoint matrix
 *
 */
qbase_matrix2 qbase_matrix_adjoint(const qbase_matrix2* mtx);


/** \brief get a inverse matrix
 *
 * \param source matrix. qbase_matrix2*
 * \return void
 *
 */
int qbase_matrix_inverse(qbase_matrix2* mtx);


/** \brief calculate the determinants of the matrix
 *
 * \param source matrix, const qbase_matrix2*
 * \return a value means the determinants of parameters
 *
 */
Real qbase_matrix_det(const qbase_matrix2* mtx);


/** \brief extend the matrix by a scalar value
 *
 * \param source matrix, qbase_matrix2*
 * \param k value Real
 * \return void
 *
 */
void qbase_matrix_extend(qbase_matrix2* mtx, Real k);


/** \brief get the result of matrix multiplication, equal to "mtx1*=mtx2"
 *
 * \param mtx1 qbase_matrix2*
 * \param mtx2 const qbase_matrix2*
 * \return no return, but modified the mtx1
 *
 */
void qbase_matrix_selfmul(qbase_matrix2* mtx1, const qbase_matrix2* mtx2);

/** \brief get the result of matrix multiplication
 *
 * \param mtx1 const qbase_matrix2*
 * \param mtx2 const qbase_matrix2*
 * \return a new matrix
 *
 */
qbase_matrix2 qbase_matrix_mul(const qbase_matrix2* mtx1, const qbase_matrix2* mtx2);

/** \brief product of matrix and vector, equal to "v *= m"
 *
 * \param vec qbase_vector*
 * \param mtx const qbase_matrix2*
 * \return modified the vector, no returns
 *
 */
qbase_vector qbase_matrix_selfvmul(qbase_vector* vec, const qbase_matrix2* mtx);

/** \brief product of matrix and vector
 *
 * \param vec const qbase_vector*
 * \param mtx const qbase_matrix2*
 * \return a new vector as return
 *
 */
qbase_vector qbase_matrix_vmul(const qbase_vector* vec, const qbase_matrix2* mtx);

#endif
