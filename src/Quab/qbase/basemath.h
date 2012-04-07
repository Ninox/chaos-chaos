#ifndef BASEMATH_H
#define BASEMATH_H

// define the Real type
typedef float Real;
typedef struct  {
    Real vec_x;
    Real vec_y;
} qb_sVector;
typedef struct  {

} qb_sMatrix2D;

// basic math
Real qbase_math_cos();
Real qbase_math_sin();
Real qbase_math_tan();
Real qbase_math_cot();
Real qbase_math_sec();
Real qbase_math_csc();
Real qbase_math_arcsin();
Real qbase_math_arccos();
Real qbase_math_arctan();
Real qbase_math_arccot();
Real qbase_math_pow();
Real qbase_math_toDegree();

// vector
void qbase_vector_reverse();
Real qbase_vector_plus();
Real qbase_vector_dot();
Real qbase_vector_cross();
bool qbase_vector_isparallel();
bool qbase_vector_isvertical();
Real qbase_vector_length();

// matrix
void qbase_matrix_reverse();
Real qbase_matrix_det();

#endif // BASEMATH.h
