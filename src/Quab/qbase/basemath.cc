#include <math.h>
#include <stdlib.h>
#include "basemath.h"

#define PI 3.1415926f
#define EPL 0.00005f

inline Real 
qbase_math_toDegree(Real rad)	{
    return 180.0f*rad/PI;
}

inline bool 
qbase_math_equal(Real val1, Real val2)	{
    // 在计算当中不一定为0则表示相等，两个数字之间误差在一定范围内也可以判定相等
    return abs(val1 - val2) <= EPL;
}


// vector
inline qbsVector 
qbase_vector_reverse(const qbsVector& vec)	{
    qbsVector v(-vec.vec_x, -vec.vec_y);
    return v;
}

inline qbsVector 
qbase_vector_plus(const qbsVector& vec1, const qbsVector& vec2)	{
    qbsVector v;
    v.vec_x = vec1.vec_x + vec2.vec_x;
    v.vec_y = vec1.vec_y + vec2.vec_y;
    return v;
}

inline Real 
qbase_vector_dot(const qbsVector& vec1, const qbsVector& vec2)	{
    return vec1.vec_x*vec2.vec_x + vec1.vec_y*vec2.vec_y;
}

inline qbsVector 
qbase_vector_cross(const qbsVector& vec1, const qbsVector& vec2)	{
}

inline Real 
qbase_vector_cos(const qbsVector& vec1, const qbsVector& vec2)   {
    return qbase_vector_dot(vec1, vec2)/((qbase_vector_length(vec1))*(qbase_vector_length(vec2)));
}

inline Real 
qbase_vector_sin(const qbsVector& vec1, const qbsVector& vec2)   {
    return 1 - pow(qbase_vector_cos(vec),2);
}

inline bool 
qbase_vector_isparallel(const qbsVector& vec1, const qbsVector& vec2)	{
    return qbase_math_equal(vec1.vec_x/vec2.vec_x, vec1.vec_y/vec2.vec_y);
}

inline bool 
qbase_vector_isvertical(const qbsVector& vec1, const qbsVector& vec2)	{
    return qbase_vector_dot(vec1, vec2) == 0;
}

inline Real
qbase_vector_length(const qbsVector& vec)	{
    return sqrt(vec.vec_x*vec.vec_x + vec.vec_y * vec.vec_y);
}


// matrix
inline void
qbase_matrix_trans(qbsMatrix2D& mtx)	{

}

inline qbsMatrix2D
qbase_matrix_adjoint(const qbsMatrix2D& mtx)	{

}

inline void
qbase_matrix_reverse(qbsMatrix2D& mtx)	{

}

inline Real
qbase_matrix_det(const qbsMatrix2D& mtx)	{

}

inline void
qbase_matrix_extend(qbsMatrix2D& mtx, Real k)	{
    mtx.m00 *= k;
    mtx.m01 *= k;
    mtx.m10 *= k;
    mtx.m11 *= k;
}

inline qbsMatrix2D
qbase_matrix_mul(const qbsMatrix2D& mtx1, const qbsMatrix2D& mtx2)	{

}

inline qbsVector
qbase_matrix_vmul(const qbsVector& vec, const qbsMatrix2D mtx)	{


}
