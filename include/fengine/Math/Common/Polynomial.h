/*
 * Polynomial header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_POLYNOMIAL_H__
#define __FORK_POLYNOMIAL_H__


#include "Math/Core/BaseMath.h"

#include <initializer_list>
#include <algorithm>
#include <array>


namespace Fork
{

namespace Math
{


/**
Generic polynomial class with (degree + 1) coefficients.
\tparam T Specifies the data type. This should be a floating-point type, e.g. float or double.
\tparam degree Specifies the polynomial's degree.
\remarks
\code
// Example of the polynomial '3t^3 - 4t^2 + 1':
Polynomial<float, 3> polynomial({ 3, -4, 0, 1 });
// polynomial[0] == 1
// polynomial[1] == 0
// polynomial[2] == -4
// polynomial[3] == 3
\endcode
*/
template <typename T, size_t degree> class Polynomial
{
    
    public:
        
        //! Number of coefficients (degree + 1).
        static const size_t numCoeff = degree + 1;

        Polynomial()
        {
            std::fill(coeff.begin(), coeff.end(), T(0));
        }
        Polynomial(const Polynomial<T, degree>& other)
        {
            std::copy(other.coeff.begin(), other.coeff.end(), coeff.begin());
        }
        Polynomial(const Polynomial<T, degree + 1>& integral)
        {
            std::copy(integral.coeff.begin() + 1, integral.coeff.end(), coeff.begin());
            for (size_t i = 1; i <= degree; ++i)
                coeff[i] *= T(i + 1);
        }
        Polynomial(const Polynomial<T, degree - 1>& derivative, const T& constant)
        {
            std::copy(derivative.coeff.begin(), derivative.coeff.end(), coeff.begin() + 1);
            coeff[0] = constant;
            for (size_t i = 1; i <= degree; ++i)
                coeff[i] /= T(i);
        }
        Polynomial(const std::initializer_list<T>& coeffInit)
        {
            /* Initialize coefficients with initializer list */
            auto it = coeff.rbegin();
            auto itInit = coeffInit.begin();

            for (; it != coeff.rend() && itInit != coeffInit.end(); ++it, ++itInit)
                *it = *itInit;

            /* Fill rest with zeros */
            std::fill(it, coeff.rend(), T(0));
        }

        /**
        Returns a reference to the specified coefficient.
        \param[in] index Specifies the coefficient index.
        Index 0 specifies the smallest coefficient, i.e. that of degree 0.
        The largest index specifies the largest coefficient, i.e. that of degree 'index'.
        */
        inline T& operator [] (const size_t index)
        {
            return coeff[index];
        }
        /**
        Returns a constant reference to the specified coefficient.
        \param[in] index Specifies the coefficient index.
        Index 0 specifies the smallest coefficient, i.e. that of degree 0.
        The largest index specifies the largest coefficient, i.e. that of degree 'index'.
        */
        inline const T& operator [] (const size_t index) const
        {
            return coeff[index];
        }

        /**
        Evaluates the polynom at the point t.
        \tparam I Specifies the interpolator data type.
        \note This is not called an "interpolation" since the point t is not limited to the range [0.0 .. 1.0].
        */
        template <typename I> T Evaluate(const I& t) const
        {
            T result = T(0);

            for (size_t i = 0; i <= degree; ++i)
                result += coeff[i] * Pow<I>(t, i);

            return result;
        }

        //! Returns the derivation (derivated by t) of this polynom.
        Polynomial<T, degree - 1> Derivate() const
        {
            return Polynomial<T, degree - 1>(*this);
        }

        //! Returns the integration (integrated over t) of this polynom.
        Polynomial<T, degree + 1> Integrate(const T& constant) const
        {
            return Polynomial<T, degree + 1>(*this, constant);
        }

        //! Array of all polynomial coefficients.
        std::array<T, numCoeff> coeff;

};


//! Specialized version of the "Polynomial" template class with degree 0.
template <typename T> class Polynomial<T, 0u>
{
    
    public:
        
        static const size_t numCoeff = 1;

        Polynomial()
        {
            coeff[0] = T(0);
        }
        Polynomial(const Polynomial<T, 0u>& other)
        {
            coeff[0] = other.coeff[0];
        }
        Polynomial(const Polynomial<T, 1u>& integral)
        {
            coeff[0] = integral.coeff[1];
        }
        Polynomial(const Polynomial<T, 0u>& derivative, const T& constant)
        {
            coeff[0] = constant;
        }
        Polynomial(const std::initializer_list<T>& coeffInit)
        {
            /* Initialize coefficients with initializer list or fill with zero */
            coeff[0] = (coeffInit.empty() ? T(0) : *coeffInit.begin());
        }

        //! Returns the specified coefficient.
        inline T& operator [] (const size_t index)
        {
            return coeff[index];
        }
        //! Returns the specified coefficient.
        inline const T& operator [] (const size_t index) const
        {
            return coeff[index];
        }

        //! Evaluates the polynom at the point t.
        template <typename I> T Evaluate(const I& t) const
        {
            return coeff[0];
        }

        //! Returns the derivation (derivated by t) of this polynom.
        Polynomial<T, 0u> Derivate() const
        {
            return Polynomial<T, 0u>();
        }

        //! Returns the integration (integrated over t) of this polynom.
        Polynomial<T, 1u> Integrate(const T& constant) const
        {
            return Polynomial<T, 1u>(*this, constant);
        }

        //! Array of all polynomial coefficients.
        std::array<T, numCoeff> coeff;

};


} // /namespace Math

} // /namespace Fork


#endif



// ========================