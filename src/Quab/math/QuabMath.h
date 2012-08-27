#ifndef QUAB_MATH_H
#define QUAB_MATH_H

#include "../QuabDef.h"

struct qbase_vector;
struct qbase_matrix2;

namespace Quab
{
    class QuabVector2D;
    class QuabMatrix2D;


    /** \brief a base math calculate helpers
     */
    class QuabMath
    {
    public:
        static Real toDegree(Real rad);
        static Real toRadian(Real deg);

        static bool isEqual(Real lhs, Real rhs);
        static bool isEqual(const QuabVector2D *v1, const QuabVector2D *v2);
        static bool isEqual(const QuabMatrix2D *m1, const QuabMatrix2D *m2);

        static Real distance(Real x1, Real y1, Real x2, Real y2);
    private:
    };

    /** \brief qbase_vector's C++ wrapper
     */
    class QuabVector2D
    {
    private:
        qbase_vector *_vec;
    public:
        QuabVector2D product(const QuabVector2D &v) const;
        QuabVector2D product(Real k) const;

        Real sin(const QuabVector2D &v) const;
        Real cos(const QuabVector2D &v) const;
        bool isParallel(const QuabVector2D &v) const;
        bool isVertical(const QuabVector2D &v) const;
        Real length() const;
    };


    /** \brief qbase_matrix2's C++ wrapper
     */
    class QuabMatrix2D
    {
    private:
        qbase_matrix2 *m;
    public:
        QuabMatrix2D(Real m00 = 0, Real m01 = 0, Real m10 = 0, Real m11 = 0);

        void setZero();
        void init(Real m00, Real m01, Real m10, Real m11);
        void transpose();
        QuabMatrix2D adjoint() const;
        void inverse();
        Real deteminant() const;
        void extend(Real k);
        QuabVector2D mulWith(const QuabVector2D &v) const;

        friend QuabMatrix2D operator +(const QuabMatrix2D &m1, const QuabMatrix2D &m2);
        friend QuabMatrix2D operator *(const QuabMatrix2D &m1, const QuabMatrix2D &m2);
    };
}
#endif
