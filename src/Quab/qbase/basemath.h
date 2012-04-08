#ifndef BASEMATH_H
#define BASEMATH_H

// define the Real type
typedef float Real;
typedef struct  {
    Real vec_x;
    Real vec_y;
} qb_sVector;
typedef struct  {
    Real m00, m01, m10, m11;
} qb_sMatrix2D;

// basic math
Real qbase_math_cos(Real deg);
Real qbase_math_sin(Real deg);
Real qbase_math_tan(Real deg);
Real qbase_math_cot(Real deg);
Real qbase_math_sec(Real deg);
Real qbase_math_csc(Real deg);
Real qbase_math_arcsin(Real val);
Real qbase_math_arccos(Real val);
Real qbase_math_arctan(Real val);
Real qbase_math_arccot(Real val);
Real qbase_math_pow(Real val, Real times);
Real qbase_math_log(Real base, Real val);
Real qbase_math_log2(Real val);
Real qbase_math_log10(Real val);
Real qbase_math_logE(Real val);
Real qbase_math_toDegree(Real rad);
bool qbase_math_equal(Real val1, Real val2);

// vector
qb_sVector qbase_vector_reverse(const qb_sVector& vec);
qb_sVector qbase_vector_plus(const qb_sVector& vec1, const qb_sVector& vec2);
Real qbase_vector_dot(const qb_sVector& vec1, const qb_sVector& vec2);
qb_sVector qbase_vector_cross(const qb_sVector& vec1, const qb_sVector& vec2);
Real qbase_vector_cos(const qb_sVector& vec1, const qb_sVector& vec2);
Real qbase_vector_sin(const qb_sVector& vec1, const qb_sVector& vec2);
bool qbase_vector_isparallel(const qb_sVector& vec1, const qb_sVector& vec2);
bool qbase_vector_isvertical(const qb_sVector& vec1, const qb_sVector& vec2);
Real qbase_vector_length(const qb_sVector& vec);

// matrix
void qbase_matrix_trans(qb_sMatrix2D& mtx);
qb_sMatrix2D qbase_matrix_adjoint(const qb_sMatrix2D& mtx);
void qbase_matrix_reverse(qb_sMatrix2D& mtx);
Real qbase_matrix_det(const qb_sMatrix2D& mtx);
void qbase_matrix_extend(qb_sMatrix2D& mtx, Real k);
qb_sMatrix2D qbase_matrix_mul(const qb_sMatrix2D& mtx1, const qb_sMatrix2D& mtx2);
qb_sVector qbase_matrix_vmul(const qb_sVector& vec, const qb_sMatrix2D mtx);

#endif // BASEMATH.h
