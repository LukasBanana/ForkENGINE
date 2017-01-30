/*
 * Math core zero header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_CORE_ZERO_H__
#define __FORK_MATH_CORE_ZERO_H__


#include "Math/Core/MathConstants.h"

#include <cmath>


namespace Fork
{

namespace Math
{


/* --- Template 'Zero' function with tolerance parameter --- */

/**
Approximated equality check with tolerance parameter.
For integer types it is not an approximation but a correct comparision (a == 0) where the tolerance value is ignored.
*/
template <typename T, typename E = T> inline bool Zero(const T& x, const E& tolerance)
{
    return x == T(0);
}

//! Approximated equality check with default tolerance parameter.
template <typename T, typename E = T> inline bool Zero(const T& x)
{
    return Zero(x, E(0));
}


/* --- Specialized 'Equal' functions --- */

template <> inline bool Zero<float, float>(const float& x, const float& tolerance)
{
    return std::abs(x) < tolerance;
}

template <> inline bool Zero<double, double>(const double& x, const double& tolerance)
{
    return std::abs(x) < tolerance;
}

/* --- Template 'Equal' function with default tolerance parameter (epsilon or epsilon64) --- */

template <> inline bool Zero<float, float>(const float& x)
{
    return Zero(x, epsilon);
}

template <> inline bool Zero<double, double>(const double& x)
{
    return Zero(x, epsilon64);
}


} // /namespace Math

} // /namespace Fork


#endif



// ========================