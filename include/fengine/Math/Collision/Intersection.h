/*
 * Intersection header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_INTERSECTION_H__
#define __FORK_MATH_INTERSECTION_H__


#include "Math/Core/Vector3.h"


namespace Fork
{

namespace Math
{


template <class V> struct Intersection
{
    V point;
    V normal;
};

template <typename T> using Intersection2 = Intersection<Vector2<T>>;
template <typename T> using Intersection3 = Intersection<Vector3<T>>;

typedef Intersection2<float> Intersection2f;
typedef Intersection2<double> Intersection2d;

typedef Intersection3<float> Intersection3f;
typedef Intersection3<double> Intersection3d;


} // /namespace Math

} // /namespace Fork


#endif



// ========================