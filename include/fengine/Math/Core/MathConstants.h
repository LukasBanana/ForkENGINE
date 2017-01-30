/*
 * Math constants header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_CONSTANTS_H__
#define __FORK_MATH_CONSTANTS_H__


namespace Fork
{

namespace Math
{


/* === Math constants === */

static const float epsilon          = 0.000001f;    //!< 32-bit floating-point rounding error value.
static const double epsilon64       = 0.00000001;   //!< 64-bit floating-point rounding error value.

static const float pi               = 3.14159265359f;
static const double pi64            = 3.1415926535897932384626433832795028841971693993751;

static const float rad2deg          = 180.0f / pi;
static const double rad2deg64       = 180.0 / pi64;

static const float deg2rad          = pi / 180.0f;
static const double deg2rad64       = pi64 / 180.0;

static const float defaultFOVdeg    = 74.0f;                        //!< Default field-of-view (FOV) in degrees.
static const double defaultFOVdeg64 = 74.0;                         //!< Default field-of-view (FOV) in degrees.

static const float defaultFOVrad    = defaultFOVdeg * deg2rad;      //!< Default field-of-view (FOV) in radians.
static const double defaultFOVrad64 = defaultFOVdeg64 * deg2rad64;  //!< Default field-of-view (FOV) in radians.


/* === Template functions for the constants === */

template <typename T> inline T Epsilon()
{
    return static_cast<T>(epsilon);
}
template <> inline float Epsilon<float>()
{
    return epsilon;
}
template <> inline double Epsilon<double>()
{
    return epsilon64;
}

template <typename T> inline T PI()
{
    return static_cast<T>(pi64);
}
template <> inline float PI<float>()
{
    return pi;
}
template <> inline double PI<double>()
{
    return pi64;
}

template <typename T> inline T Rad2Deg()
{
    return static_cast<T>(rad2deg64);
}
template <> inline float Rad2Deg<float>()
{
    return rad2deg;
}
template <> inline double Rad2Deg<double>()
{
    return rad2deg64;
}

template <typename T> inline T Deg2Rad()
{
    return static_cast<T>(deg2rad64);
}
template <> inline float Deg2Rad<float>()
{
    return deg2rad;
}
template <> inline double Deg2Rad<double>()
{
    return deg2rad64;
}

template <typename T> inline T DefaultFOVdeg()
{
    return static_cast<T>(defaultFOVdeg64);
}
template <> inline float DefaultFOVdeg<float>()
{
    return defaultFOVdeg;
}
template <> inline double DefaultFOVdeg<double>()
{
    return defaultFOVdeg64;
}

template <typename T> inline T DefaultFOVrad()
{
    return static_cast<T>(defaultFOVrad64);
}
template <> inline float DefaultFOVrad<float>()
{
    return defaultFOVrad;
}
template <> inline double DefaultFOVrad<double>()
{
    return defaultFOVrad64;
}


} // /namespace Math

} // /namespace Fork


#endif



// ========================