#include <math.h>
#include <stdlib.h>
#include "basemath.h"

#define PI 3.1415926f

// basic math
Real qbase_math_cos(Real deg)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return cos(deg);
#endif
#endif
}

Real qbase_math_sin(Real deg)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return sin(deg);
#endif
#endif
}

Real qbase_math_tan(Real deg)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return tan(deg);
#endif
#endif
}

Real qbase_math_cot(Real deg)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return 1.0f/tan(deg);
#endif
#endif
}

Real qbase_math_sec(Real deg)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return 1.0f/sin(deg);
#endif
#endif
}

Real qbase_math_csc(Real deg)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return 1.0f/cos(deg);
#endif
#endif
}

Real qbase_math_arcsin(Real val)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return asin(val);
#endif
#endif
}

Real qbase_math_arccos(Real val)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return acos(val);
#endif
#endif
}

Real qbase_math_arctan(Real val)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return atan(val);
#endif
#endif
}

Real qbase_math_arccot(Real val)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return atan(1.0f/val);
#endif
#endif
}

Real qbase_math_pow(Real val, Real times)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return pow(val, times);
#endif
#endif
}

Real qbase_math_log(Real base, Real val)    {
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return log(val)/log(base);
#endif
#endif
}

Real qbase_math_log2(Real val)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return log(val)/log(2);
#endif
#endif
}

Real qbase_math_log10(Real val)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return log10(val);
#endif
#endif
}

Real qbase_math_logE(Real val)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return log(val);
#endif
#endif
}

Real qbase_math_toDegree(Real rad)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return 180.0f*rad/PI;
#endif
#endif
}

bool qbase_math_equal(Real val1, Real val2)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    // 在计算当中不一定为0则表示相等，两个数字之间误差在一定范围内也可以判定相等
    return abs(val1 - val2) <= 0.00005f;
#endif
#endif
}


// vector
qb_sVector qbase_vector_reverse(const qb_sVector& vec)	{
    qb_sVector v(-vec.vec_x, -vec.vec_y);
    return v;
}

qb_sVector qbase_vector_plus(const qb_sVector& vec1, const qb_sVector& vec2)	{
    qb_sVector v;
    v.vec_x = vec1.vec_x + vec2.vec_x;
    v.vec_y = vec1.vec_y + vec2.vec_y;
    return v;
}

Real qbase_vector_dot(const qb_sVector& vec1, const qb_sVector& vec2)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return vec1.vec_x*vec2.vec_x + vec1.vec_y*vec2.vec_y;
#endif
#endif
}

qb_sVector qbase_vector_cross(const qb_sVector& vec1, const qb_sVector& vec2)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else

#endif
#endif
}

Real qbase_vector_cos(const qb_sVector& vec1, const qb_sVector& vec2)   {
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else

#endif
#endif
}

Real qbase_vector_sin(const qb_sVector& vec1, const qb_sVector& vec2)   {
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else

#endif
#endif
}

bool qbase_vector_isparallel(const qb_sVector& vec1, const qb_sVector& vec2)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return qbase_math_equal(vec1.vec_x/vec2.vec_x, vec1.vec_y/vec2.vec_y);
#endif
#endif
}

bool qbase_vector_isvertical(const qb_sVector& vec1, const qb_sVector& vec2)	{
    return qbase_vector_dot(vec1, vec2) == 0;
}

Real qbase_vector_length(const qb_sVector& vec)	{
#ifdef QUAB_CL_MSVC
    __asm   {

    }
#else
#ifdef QUAB_PL_X86
#else
    return sqrt(vec.vec_x*vec.vec_x + vec.vec_y * vec.vec_y);
#endif
#endif
}


// matrix
void qbase_matrix_trans(qb_sMatrix2D& mtx)	{

}

qb_sMatrix2D qbase_matrix_adjoint(const qb_sMatrix2D& mtx)	{

}

void qbase_matrix_reverse(qb_sMatrix2D& mtx)	{

}

Real qbase_matrix_det(const qb_sMatrix2D& mtx)	{

}

void qbase_matrix_extend(qb_sMatrix2D& mtx, Real k)	{
    mtx.m00 *= k;
    mtx.m01 *= k;
    mtx.m10 *= k;
    mtx.m11 *= k;
}

qb_sMatrix2D qbase_matrix_mul(const qb_sMatrix2D& mtx1, const qb_sMatrix2D& mtx2)	{

}

qb_sVector qbase_matrix_vmul(const qb_sVector& vec, const qb_sMatrix2D mtx)	{


}
