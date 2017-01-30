/*
 * Convert functions header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_COMMON_CONVERT_FUNCTIONS_H__
#define __FORK_MATH_COMMON_CONVERT_FUNCTIONS_H__


#include "Math/Core/Vector2.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/Vector4.h"
#include "Math/Core/Size2.h"
#include "Math/Core/Size3.h"


namespace Fork
{

namespace Math
{


/* --- Vector to vector --- */

template <typename T> inline Vector2<T> ToVec2(const Vector2<T>& vec)
{
    return vec;
}
template <typename T> inline Vector2<T> ToVec2(const Vector3<T>& vec)
{
    return vec.Vec2();
}
template <typename T> inline Vector2<T> ToVec2(const Vector4<T>& vec)
{
    return vec.Vec2();
}

template <typename T> inline Vector3<T> ToVec3(const Vector2<T>& vec)
{
    return { vec.x, vec.y, 0 };
}
template <typename T> inline Vector3<T> ToVec3(const Vector3<T>& vec)
{
    return vec;
}
template <typename T> inline Vector3<T> ToVec3(const Vector4<T>& vec)
{
    return vec.Vec3();
}

template <typename T> inline Vector4<T> ToVec4(const Vector2<T>& vec)
{
    return { vec.x, vec.y, 0, 1 };
}
template <typename T> inline Vector4<T> ToVec4(const Vector3<T>& vec)
{
    return { vec.x, vec.y, vec.z, 1 };
}
template <typename T> inline Vector4<T> ToVec4(const Vector4<T>& vec)
{
    return vec;
}

/* --- Size to vector --- */

template <typename T> inline Vector2<T> ToVec2(const Size2<T>& size)
{
    return { size.width, size.height };
}

template <typename T> inline Vector3<T> ToVec3(const Size3<T>& size)
{
    return { size.width, size.height, size.depth };
}


} // /namespace Math

} // /namespace Fork


#endif



// ========================