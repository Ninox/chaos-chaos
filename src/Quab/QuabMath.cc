#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "math/QuabMath.h"
#include "qbase/qbase.h"
using namespace Quab;

/*  implement the QuabMath methods  */
Real QuabMath::PI = 3.1415926f;
Real QuabMath::toDegree(Real rad)	{
	return qbase_math_todegree(rad);
}
Real QuabMath::toRadian(Real deg)	{
	return deg * QuabMath::PI/180;
}

bool QuabMath::isEqual(Real lhs, Real rhs)	{
	return qbase_math_equal(lhs, rhs) == 1;
}
bool QuabMath::isEqual(const QuabVector2D *v1, const QuabVector2D *v2)	{
	return qbase_math_equalV(v1->_vec, v2->_vec) == 1;
}
bool QuabMath::isEqual(const QuabMatrix2D *m1, const QuabMatrix2D *m2)	{
	return qbase_math_equalM(m1->m, m2->m) == 1;
}

/*  implements of QuabVector2D's method  */
QuabVector2D::QuabVector2D(Real x, Real y)	{
	this->_vec = (qbase_vector*)malloc(sizeof(qbase_vector));
	this->_vec->posX = x;
	this->_vec->posY = y;
	this->_vec->standard = V_ROW;
}
QuabVector2D::~QuabVector2D()	{
	if(this->_vec != NULL)	{
		free(this->_vec);
		this->_vec = NULL;
	}
}

inline void QuabVector2D::set(Real x, Real y)	{
	this->_vec->posX = x;
	this->_vec->posY = y;
}
inline Real QuabVector2D::getX()	{
	return this->_vec->posX;
}
inline Real QuabVector2D::getY()	{
	return this->_vec->posY;
}
void QuabVector2D::extend(Real k)	{
	if(this->_vec != NULL)
		qbase_vector_mul(this->_vec, k);
}

Real QuabVector2D::sin(const QuabVector2D &v) const	{
	return qbase_vector_sin(this->_vec, v._vec);
}
Real QuabVector2D::cos(const QuabVector2D &v) const	{
	return qbase_vector_cos(this->_vec, v._vec);
}
bool QuabVector2D::isParallel(const QuabVector2D &v) const	{
	return qbase_vector_isparallel(this->_vec, v._vec) == 1;
}
bool QuabVector2D::isVertical(const QuabVector2D &v) const	{
	return qbase_vector_isvertical(this->_vec, v._vec);
}
Real QuabVector2D::length() const	{
	return qbase_vector_length(this->_vec);
}

QuabVector2D QuabVector2D::operator +(const QuabVector2D &v)	{
	qbase_vector *vec = (qbase_vector*)malloc(sizeof(qbase_vector));
	qbase_vector result = qbase_vector_plus(this->_vec, v._vec);
	*vec = result;
	return QuabVector2D(vec);
}
Real QuabVector2D::operator *(const QuabVector2D &v)	{
	return qbase_vector_dot(this->_vec, v._vec);
}

QuabVector2D& QuabVector2D::operator =(const QuabVector2D &v)	{
	if(this->_vec != NULL)
		free(this->_vec);
	return  const_cast<QuabVector2D&>(v);
}

/*  implemention of QuabMatrix2D */
QuabMatrix2D::QuabMatrix2D(const qbase_matrix2 *mtx)	{
	this->m = (qbase_matrix2*)malloc(sizeof(qbase_matrix2));
	*(this->m) = *mtx;
}
QuabMatrix2D::QuabMatrix2D(Real m00, Real m01, Real m10, Real m11)	{
	this->m = (qbase_matrix2*)malloc(sizeof(qbase_matrix2));
	qbase_matrix_init(this->m, V_ROW, m00, m01, m10, m11);
}
QuabMatrix2D::~QuabMatrix2D()	{
	free(this->m);
}

inline void QuabMatrix2D::set(int row, int col, Real val)	{
	assert(row <= 3 && row >= 0 && col >= 0 && col <=3);
	this->m->m[row][col] = val;
}
inline Real QuabMatrix2D::get(int row, int col) const	{
	assert(row <= 3 && row >= 0 && col >= 0 && col <=3);
	return this->m->m[row][col];
}
inline void QuabMatrix2D::setZero()	{
	qbase_matrix_zero(this->m, V_ROW);
}
void QuabMatrix2D::init(Real m00, Real m01, Real m10, Real m11)	{
	qbase_matrix_init(this->m, V_ROW, m00, m01, m10, m11);
}

void QuabMatrix2D::transpose()	{
	qbase_matrix_trans(this->m);
}
QuabMatrix2D QuabMatrix2D::adjoint() const	{
	qbase_matrix2 adjMatrix = qbase_matrix_adjoint(this->m);
	return QuabMatrix2D(&adjMatrix);
}
void QuabMatrix2D::inverse()	{
	qbase_matrix_inverse(this->m);
}
Real QuabMatrix2D::deteminant() const	{
	return qbase_matrix_det(this->m);
}
void QuabMatrix2D::extend(Real k)	{
	qbase_matrix_extend(this->m, k);
}
QuabVector2D QuabMatrix2D::mulWith(const QuabVector2D &v)	{
	 qbase_vector vec = qbase_matrix_vmul(v._vec, this->m);
	 return QuabVector2D(vec.posX, vec.posY);
}

QuabMatrix2D QuabMatrix2D::operator +(const QuabMatrix2D &matrix)	{
	qbase_matrix2 mtx = qbase_matrix_plus(this->m, matrix.m);
	return QuabMatrix2D(mtx.m[0][0],mtx.m[0][1],mtx.m[1][0],mtx.m[1][1]);
}
QuabMatrix2D QuabMatrix2D::operator *(const QuabMatrix2D &matrix)	{
	qbase_matrix2 mtx = qbase_matrix_mul(this->m, matrix.m);
	return QuabMatrix2D(mtx.m[0][0], mtx.m[0][1], mtx.m[1][0], mtx.m[1][1]);
}
QuabMatrix2D& QuabMatrix2D::operator=(const QuabMatrix2D& matrix)	{
	if(this->m != NULL)	{
		free(this->m);
	}
	return const_cast<QuabMatrix2D&>(matrix);
}