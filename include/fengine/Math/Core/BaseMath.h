/*
 * Base math header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_BASE_MATH_H__
#define __FORK_BASE_MATH_H__


namespace Fork
{

namespace Math
{


/* --- Template functions -- */

/**
Signum function. It returns the sign of the specified value 'x'.
\param[in] x Specifies the input value.
\return 0 if x = 0; -1 if x < 0; 1 if x > 0.
*/
template <typename T> inline T Sgn(const T& x)
{
    if (x == T(0))
        return T(0);
    return x > 0 ? T(1) : T(-1);
}

//! Squares the input parameter, i.e. x*x.
template <typename T> inline T Sq(const T& x)
{
    return x*x;
}

//! Inverses the specified value with the template argument "invVal" (e.g. Inv<float, 1>(0.7) = 0.3 and Inv<unsigned char, 255>(200) = 55).
template <int invVal, typename T> inline T Inv(const T& x)
{
    return T(invVal) - x;
}

/**
Returns (base ^ exponent). Here the exponent is an unsigned integer value.
\todo Add 'constexpr' as soon as VisualC++ supports it.
*/
template <typename T> /*constexpr*/ T Pow(const T& base, size_t exponent)
{
    return exponent == 0 ? 1 : base * Pow(base, exponent - 1);
}

/**
Returns the input value, clamped between the specified minimum and maximum.
\param[in] x Specifies the input value.
\param[in] min Specifies the minimum.
\param[in] max Specifies the maximum.
\return x in the range min and max, i.e. min >= Clamp(x, min, max) <= max.
*/
template <typename T> inline T Clamp(const T& x, const T& min, const T& max)
{
    if (x > max)
        return max;
    if (x < min)
        return min;
    return x;
}

/**
Returns the saturated input value. This is equivalent to the following code:
\code
Clamp(x, 0, 1);
\endcode
*/
template <typename T> inline T Saturate(const T& x)
{
    return Clamp(x, T(0), T(1));
}

/**
Returns the saturated input value within the specified range.
\return 0.0 if x <= min, 1.0 if x >= max and a linear interpolation between 0.0 and 1.0 if x is an element of [min .. max].
*/
template <typename T> inline T Saturate(const T& x, const T& min, const T& max)
{
    return Saturate((x - min) / (max - min));
}

/**
Transform the input value x from the range [min .. max] to [0 .. 1]. This is equivalent to the following (pseudo) code:
\code
x' := (x - min) / (max - min)
\endcode
\tparam T Specifies the data type. Should be float or double. If a class type is used,
it must implement the -= and /= operators.
*/
template <typename T> inline T TransformSaturate(T x, const T& min, T max)
{
    /* Make a saturate transformation as 'y := (x - min) / (max - min)' */
    x -= min;
    max -= min;
    x /= max;
    return x;
}

/**
Returns a linear interpolation (Linear intERPolation -> Lerp).
\tparam T Specifies the point type. This type must implement the following operators:
\code
// I denotes the other template argument.
T = T, T -= T, T *= I, T += T;
\endcode
\tparam I Specifies the interpolation factor type. This should be float or double.
Returns the linear interpolation between the two points a and b with the factor t.
\param[in] from Specifies the start point.
\param[in] to Specifies the end point.
\param[in] t Specifies the interpolation factor. This should be in the range [0.0 .. 1.0].
\return Linear interpolation: Lerp := a + (b - a)*t = a*(1 - t) + b*t.
*/
template <typename T, typename I> inline void Lerp(T& result, const T& from, const T& to, const I& t)
{
    /* Compute linear interpolation as 'Lerp := from + (to - from)*t' */
    result = to;
    result -= from;
    result *= t;
    result += from;
}

//! \see Lerp(T& result, const T& from, const T& to, const I& t)
template <typename T, typename I> inline T Lerp(const T& from, const T& to, const I& t)
{
    T result;
    Lerp(result, from, to, t);
    return result;
}

/**
Returns a smooth hermite interpolation (Hermiate intERPolation -> Herp) between 0 and 1, if x is in the range [0.0 .. 1.0].
\return Cubic hermite interpolation: Herp := 3x^2 - 2x^3.
*/
template <typename T> inline T SmoothStep(const T& x)
{
    /* Compute hermite interpolation and evaluate polynomial */
    return x*x * (T(3) - x*T(2));
}

/**
Returns a smooth hermite interpolation (Hermiate intERPolation -> Herp) between 0 and 1, if x is in the range [min .. max].
\return Cubic hermite interpolation: Herp := 3x^2 - 2x^3.
\note The parameters are (min, max, x) and not (x, min, max) like in the "Clamp" function.
That's because the interface between shader languages (GLSL and HLSL) and this graphics engine should be the same.
*/
template <typename T> inline T SmoothStep(const T& min, const T& max, T x)
{
    /* Compute hermite interpolation and evaluate polynomial */
    return SmoothStep(Saturate(x, min, max));
}

/**
Returns a smooth hermite interpolation between 0 and 1, if x is in the range [0.0 .. 1.0].
This is an improved version of the "SmoothStep" function but a little slower.
\return Hermite interpolation: Herp := 6x^5 - 15x^4 + 10x^3.
\see SmoothStep
*/
template <typename T> inline T SmootherStep(const T& x)
{
    return x*x*x * (x*(x*T(6) - T(15)) + T(10));
}

/**
Returns a smooth hermite interpolation between 0 and 1, if x is in the range [min .. max].
This is an improved version of the "SmoothStep" function but a little slower.
\return Hermite interpolation: Herp := 6x^5 - 15x^4 + 10x^3.
\see SmoothStep
*/
template <typename T> inline T SmootherStep(const T& min, const T& max, T x)
{
    /* Compute hermite interpolation and evaluate polynomial */
    return SmootherStep(Saturate(x, min, max));
}


/* --- Constant expression functions --- */

/**
Computes the factorial of the specified value n.
\todo Add 'constexpr' as soon as VisualC++ supports it.
*/
template <typename dummy = unsigned int>/*constexpr */unsigned int Factorial(unsigned int n)
{
    return n > 1 ? n * Factorial<dummy>(n - 1) : 1;
}

/**
Computes the binominal coefficient of the n over k.
\todo Add 'constexpr' as soon as VisualC++ supports it.
*/
template <typename dummy = unsigned int>/*constexpr*/ unsigned int BinomCoeff(unsigned int n, unsigned int k)
{
    return k > n ? 0 : Factorial<dummy>(n) / (Factorial<dummy>(k) * Factorial<dummy>(n - k));
}

/**
Computes the bernstein polynomial of degree n at point t.
\tparam T Specifies the data type. Should be float or double.
\param[in] i Specifies the index for which applies: 0 <= i <= n.
\param[in] n Specifies the degree of the bernstein polynomial.
\todo Add 'constexpr' as soon as VisualC++ supports it.
*/
template <typename T> /*constexpr*/ T ComputeBernsteinPolynomial(unsigned int i, unsigned int n, const T& t)
{
    return static_cast<T>(std::pow(t, i) * std::pow(T(1) - t, n - i) * BinomCoeff(n, i));
}

/**
Computes the efficient modular power-of value.
\tparam T Specifies the data type for the computation.
This type must support the operators %, %=, *= and >>=.
Therefore it can not be used with floating-point types.
\param[in] base Specifies the base 'b' of the computation: (b ^ e) mod m.
\param[in] expr Specifies the exponent 'e' of the computation: (b ^ e) mod m.
\param[in] modulus Specifies the modulus 'm' of the computation: (b ^ e) mod m.
*/
template <typename T> /*constexpr*/ T ModPow(T base, T exp, const T& modulus)
{
    T result = 1;
    
    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            result *= base;
            result %= modulus;
        }
        exp >>= 1;
        base *= base;
        base %= modulus;
    }
    
    return result;
};


} // /namespace Math

} // /namespace Fork


#endif



// ========================