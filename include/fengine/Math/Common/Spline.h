/*
 * Spline header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SPLINE_H__
#define __FORK_SPLINE_H__


#include "Math/Common/Polynomial.h"
#include "Math/Core/Vector2.h"
#include "Math/Core/Vector3.h"
#include "Core/Exception/IndexOutOfBoundsException.h"

#include <vector>
#include <cmath>


namespace Fork
{

namespace Math
{


/**
Generic spline class.
\tparam V Specifies the class for the control points. This should be Vector2f, Vector2d, Vector3f or Vector3d.
\tparam I Specifies the data type of the interpolator. This should be float or double.
\tparam dimension Specifies the dimension of the control points. This can be set manual,
so that 1-dimensional splines can be used, too (without a separate vector class).
*/
template <class V, typename I, size_t dimension> class Spline
{
    
    public:
        
        //! Spline's polynomial type.
        typedef Polynomial<V, 3> PolynomialType;

        /**
        Builds the spline polynomials.
        \param[in] points Specifies the control points.
        \param[in] expansion Specifies the expansion of the polynomials.
        If the expansion is 0.0, this spline will be a linear spline. By default 1.0.
        */
        void Build(const std::vector<V>& points, const I& expansion = I(1))
        {
            if (points.size() >= 2)
            {
                polynomials_.resize(points.size() - 1);

                for (size_t i = 0; i < dimension; ++i)
                    BuildDimension(points, i, expansion);
            }
        }

        //! Clears the spline polynoms.
        void Clear()
        {
            polynomials_.clear();
        }

        /**
        Interpolates this spline.
        \param[in] t Specifies the interpolation time point in the range [0.0 .. 1.0].
        */
        V Interpolate(I t) const
        {
            if (NumPolynoms() >= 1)
            {
                /* Clamp to edges */
                if (t <= I(0))
                    return GetPolynom(0).Evaluate(I(0));
                else if (t >= I(1))
                    return GetPolynom(NumPolynomials() - 1).Evaluate(I(1));

                /* Get polynomial index and transform interpolator */
                t *= NumPolynomials();
                
                const I trimedT = std::floor(t);
                t -= trimedT;

                const size_t index = static_cast<size_t>(trimedT);

                /* Return polynom interpolation */
                return GetPolynomial(index).Evaluate(t);
            }
            return V(0);
        }

        /**
        Returns a reference to the specified spline polynomial.
        \throws IndexOutOfBoundsException if the index is out of bounds and the engine was compiled with debug information.
        \see PolynomType
        */
        inline PolynomialType& GetPolynomial(const size_t index)
        {
            #ifdef FORK_DEBUG
            if (index >= polynomials_.size())
                throw IndexOutOfBoundsException(__FUNCTION__, index);
            #endif
            return polynomials_[index];
        }

        /**
        Returns a constant reference to the specified spline polynomial.
        \throws IndexOutOfBoundsException if the index is out of bounds and the engine was compiled with debug information.
        \see PolynomType
        */
        inline const PolynomialType& GetPolynomial(const size_t index) const
        {
            #ifdef FORK_DEBUG
            if (index >= polynomials_.size())
                throw IndexOutOfBoundsException(__FUNCTION__, index);
            #endif
            return polynomials_[index];
        }

        //! Returns the number of polynomials.
        inline size_t NumPolynomials() const
        {
            return polynomials_.size();
        }

        /**
        Returns the polynomial list.
        \see Math::Polynomial
        */
        inline const std::vector<PolynomialType>& GetPolynomials() const
        {
            return polynomials_;
        }

    private:
        
        /* === Functions === */

        //! Builds the polynomials for the specified dimension.
        void BuildDimension(const std::vector<V>& points, const size_t dim, const I& expansion)
        {
            const size_t numPoints = points.size();

            std::vector<I> s(numPoints), y(numPoints), v(numPoints), q(numPoints);

            y[0] = 3 * ((points[1])[dim] - (points[0])[dim]);

            for (size_t i = 1; i < numPoints - 1; ++i)
                y[i] = 3 * ((points[i+1])[dim] - (points[i-1])[dim]);

            y[numPoints - 1] = 3 * ((points[numPoints - 1])[dim] - (points[numPoints - 2])[dim]);

            v[0] = I(0.5);
            q[0] = I(0.5) * y[0];

            for (size_t i = 1; i + 1 < numPoints; ++i)
            {
                v[i] = I(1) / (4 - v[i - 1]);
                q[i] = expansion * v[i] * (y[i] - q[i - 1]);
            }

            q[numPoints - 1] = expansion * (I(1) / (2 - v[numPoints - 2])) * (y[numPoints - 1] - q[numPoints - 2]);

            s[numPoints - 1] = q[numPoints - 1];

            for (size_t i = numPoints - 1; i > 0; --i)
                s[i - 1] = q[i - 1] - v[i - 1]*s[i];

            for (size_t i = 0; i + 1 < numPoints; ++i)
            {
                auto& polynomial = polynomials_[i];

                (polynomial[0])[dim] = (points[i])[dim];
                (polynomial[1])[dim] = s[i];
                (polynomial[2])[dim] = 3*(points[i+1])[dim] - 3*(points[i  ])[dim] - 2*s[i] - s[i+1];
                (polynomial[3])[dim] = 2*(points[i  ])[dim] - 2*(points[i+1])[dim] +   s[i] + s[i+1];
            }
        }

        /* === Members === */

        std::vector<PolynomialType> polynomials_;

};


typedef Spline<Vector2f, float, 2> Spline2f;
typedef Spline<Vector3f, float, 3> Spline3f;

typedef Spline<Vector2d, double, 2> Spline2d;
typedef Spline<Vector3d, double, 3> Spline3d;


} // /namespace Math

} // /namespace Fork


#endif



// ========================