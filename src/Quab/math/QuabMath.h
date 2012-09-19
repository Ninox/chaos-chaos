#ifndef QUAB_MATH_H
#define QUAB_MATH_H

#include <math.h>
#include "../QuabDef.h"

struct qbase_vector;
struct qbase_matrix2;

namespace Quab
{
    class QuabVector2D;
    class QuabMatrix2D;

    /** \brief a base math calculate helpers
     */
    class QUAB_API QuabMath
    {
    public:
		static Real PI;
	
        static Real toDegree(Real rad);
        static Real toRadian(Real deg);

        static bool isEqual(Real lhs, Real rhs);
        static bool isEqual(const QuabVector2D *v1, const QuabVector2D *v2);
        static bool isEqual(const QuabMatrix2D *m1, const QuabMatrix2D *m2);

        static inline Real distance(Real x1, Real y1, Real x2, Real y2)	{
			return (Real)sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
		}
    };

    /** \brief qbase_vector's C++ wrapper
     */
    class QUAB_API QuabVector2D
    {
    private:
        qbase_vector *_vec;
		QuabVector2D(qbase_vector *vec) { this->_vec = vec; }
    public:
		QuabVector2D(Real x = 0, Real y = 0);
		~QuabVector2D();
        void extend(Real k);

		inline void set(Real x, Real y);
		inline Real getX();
		inline Real getY();
        Real sin(const QuabVector2D &v) const;
        Real cos(const QuabVector2D &v) const;
        bool isParallel(const QuabVector2D &v) const;
        bool isVertical(const QuabVector2D &v) const;
        Real length() const;
		
		QuabVector2D operator +(const QuabVector2D &v);
		Real operator*(const QuabVector2D &v);
		
		friend class QuabMath;
		friend class QuabMatrix2D;
    };


    /** \brief qbase_matrix2's C++ wrapper
     */
    class QUAB_API QuabMatrix2D
    {
    private:
        qbase_matrix2 *m;
		QuabMatrix2D(const qbase_matrix2 *mtx);
    public:
        QuabMatrix2D(Real m00 = 0, Real m01 = 0, Real m10 = 0, Real m11 = 0);
		~QuabMatrix2D();
		
        inline void setZero();
        inline void init(Real m00, Real m01, Real m10, Real m11);
		inline Real get(int row, int col) const;
		inline void set(int row, int col, Real val);
		
        void transpose();
        QuabMatrix2D adjoint() const;
        void inverse();
        Real deteminant() const;
        void extend(Real k);
        QuabVector2D mulWith(const QuabVector2D &v);

        QuabMatrix2D operator+ (const QuabMatrix2D &matrix);
        QuabMatrix2D operator* (const QuabMatrix2D &matrix);
		
		friend class QuabMath;
    };
}
#endif
