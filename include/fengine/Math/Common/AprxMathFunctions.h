/*
 * Approximated math function header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_COMMON_APRX_MATH_FUNCTIONS_H__
#define __FORK_MATH_COMMON_APRX_MATH_FUNCTIONS_H__


#include "Core/Export.h"


namespace Fork
{

namespace Math
{

//! Namespace for faster but approximated math functions.
namespace Aprx
{


/**
Returns the fast inverse-square-root function from Quake III.
In comparision to the "std::sqrt" function the approximation is good enough for most computer graphics effects.
\code
// Precision examples:
   1.0f / std::sqrt(9.0f) = 0.333333
Math::Aprx::InvSqrt(9.0f) = 0.332953

   1.0f / std::sqrt(2.0f) = 0.707107
Math::Aprx::InvSqrt(2.0f) = 0.70693

   1.0f / std::sqrt(48.0064f) = 0.144238
Math::Aprx::InvSqrt(48.0064f) = 0.144114
\endcode
\see http://en.wikipedia.org/wiki/Fast_inverse_square_root
*/
FORK_EXPORT float InvSqrt(float x);

/**
Fast and accurate sine function approximation.
This function is around 8 times faster than the "std::sin" function.
\code
// Precision examples:
       std::sin(-9.0f) = -0.412118
Math::Aprx::Sin(-9.0f) = -0.4117

       std::sin(0.5f) = 0.479426
Math::Aprx::Sin(0.5f) = 0.479329

       std::sin(2.0f) = 0.909297
Math::Aprx::Sin(2.0f) = 0.909796
\endcode
\param[in] x Specifies the angle value (in radians) for which the sine is to be computed.
\see http://lab.polygonal.de/?p=205
*/
FORK_EXPORT float Sin(float x);

/**
Fast and accurate cosine function approximation.
\param[in] x Specifies the angle value (in radians) for which the cosine is to be computed.
\see Math::Aprx::Sin
*/
FORK_EXPORT float Cos(float x);

/**
Fast and approximated logarithm of base 2.
\see http://stackoverflow.com/questions/9411823/fast-log2float-x-implementation-c
*/
FORK_EXPORT float Log2(float x);

/**
Fast and approximated logarithm of base e (natural logarithm).
\see Log2
*/
FORK_EXPORT float Log(float x);

/**
Fast and accurate integral logarithm of base 2.
\see Log
*/
FORK_EXPORT int ILog2(float x);


} // /namespace Aprx

} // /namespace Math

} // /namespace Fork


#endif



// ========================