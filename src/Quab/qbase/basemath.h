#ifndef BASEMATH_H
#define BASEMATH_H

// define the Real type
typedef float Real;
typedef struct  {
    Real vec_x;
    Real vec_y;
} qbsVector;
typedef struct  {
    Real m00, m01, m10, m11;
} qbsMatrix2D;

// basic math
inline Real qbase_math_toDegree(Real rad);
inline bool qbase_math_equal(Real val1, Real val2);

// vector
inline qbsVector qbase_vector_reverse(const qbsVector& vec);
inline qbsVector qbase_vector_plus(const qbsVector& vec1, const qbsVector& vec2);
inline Real qbase_vector_dot(const qbsVector& vec1, const qbsVector& vec2);
inline qbsVector qbase_vector_cross(const qbsVector& vec1, const qbsVector& vec2);
inline Real qbase_vector_cos(const qbsVector& vec1, const qbsVector& vec2);
inline Real qbase_vector_sin(const qbsVector& vec1, const qbsVector& vec2);
inline bool qbase_vector_isparallel(const qbsVector& vec1, const qbsVector& vec2);
inline bool qbase_vector_isvertical(const qbsVector& vec1, const qbsVector& vec2);
inline Real qbase_vector_length(const qbsVector& vec);

// matrix
inline void qbase_matrix_trans(qbsMatrix2D& mtx);
inline qbsMatrix2D qbase_matrix_adjoint(const qbsMatrix2D& mtx);
inline void qbase_matrix_reverse(qbsMatrix2D& mtx);
inline Real qbase_matrix_det(const qbsMatrix2D& mtx);
inline void qbase_matrix_extend(qbsMatrix2D& mtx, Real k);
inline qbsMatrix2D qbase_matrix_mul(const qbsMatrix2D& mtx1, const qbsMatrix2D& mtx2);
inline qbsVector qbase_matrix_vmul(const qbsVector& vec, const qbsMatrix2D mtx);

#endif // BASEMATH.h
