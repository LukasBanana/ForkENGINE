/*
 * Approximated math function file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Math/Common/AprxMathFunctions.h"
#include "Math/Core/MathConstants.h"

#include <cmath>


namespace Fork
{

namespace Math
{

namespace Aprx
{


FORK_EXPORT float InvSqrt(float x)
{
    static const float threeHalfs = 1.5f;

    float x2 = x * 0.5f;
    float y = x;
    long i = *reinterpret_cast<long*>(&y);  // Evil floating point bit level hacking
    i = 0x5f3759df - (i >> 1);              // Magic number
    y = *reinterpret_cast<float*>(&i);
    y = y * (threeHalfs - (x2*y*y));        // 1st iteration

    return y;
}

FORK_EXPORT float Sin(float x)
{
    /* Always wrap input angle to [-PI .. PI] */
    if (x < -Math::pi)
        x = std::fmod(x - Math::pi, Math::pi*2.0f) + Math::pi;
    else if (x > Math::pi)
        x = std::fmod(x + Math::pi, Math::pi*2.0f) - Math::pi;

    /* Compute sine */
    float y;

    if (x < 0)
    {
        y = 1.27323954f * x + 0.405284735f * x*x;

        if (y < 0)
            y = 0.225f * (y*(-y) - y) + y;
        else
            y = 0.225f * (y*y - y) + y;
    }
    else
    {
        y = 1.27323954f * x - 0.405284735f * x*x;

        if (y < 0)
            y = 0.225f * (y*(-y) - y) + y;
        else
            y = 0.225f * (y*y - y) + y;
    }

    return y;
}

FORK_EXPORT float Cos(float x)
{
    return Sin(x + Math::pi*0.5f);
}

FORK_EXPORT float Log2(float x)
{
    long* const expPtr = (long*)(&x);
    long y = *expPtr;
    const long lg2 = ((y >> 23) & 255) - 128;
    
    y &= ~(255 << 23);
    y += 127 << 23;
    
    *expPtr = y;

    x = ((-1.0f/3) * x + 2) * x - 2.0f/3;

    return (x + lg2);
}

FORK_EXPORT float Log(float x)
{
    return Log2(x) * 0.69314718f;
}

FORK_EXPORT int ILog2(float x)
{
    int y;
    std::frexp(x, &y);
    return y - 1;
}


} // /namespace Aprx

} // /namespace Math

} // /namespace Fork



// ========================