/*
 * Vector/size counterparts header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_VECTOR_SIZE_COUNTERPARTS_H__
#define __FORK_VECTOR_SIZE_COUNTERPARTS_H__


#include "Math/Core/Vector2.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/Size2.h"
#include "Math/Core/Size3.h"


namespace Fork
{

namespace Math
{


/* --- Vector counterpart --- */

//! Abstract vector/size counterpart
template < template <typename> class VecT, typename T > struct VectorCounterpart
{
    typedef VecT<T> SizeType;
};

//! Vector2 size counterpart: Size2
template <typename T> struct VectorCounterpart<Vector2, T>
{
    typedef Size2<T> SizeType;
};

//! Vector3 size counterpart: Size3
template <typename T> struct VectorCounterpart<Vector3, T>
{
    typedef Size3<T> SizeType;
};


/* --- Size counterpart --- */

//! Abstract size/vector counterpart
template < template <typename> class SzT, typename T > struct SizeCounterpart
{
    typedef SzT<T> VectorType;
};

//! Size2 size counterpart: Vector2
template <typename T> struct SizeCounterpart<Size2, T>
{
    typedef Vector2<T> VectorType;
};

//! Size3 size counterpart: Vector3
template <typename T> struct SizeCounterpart<Size3, T>
{
    typedef Vector3<T> VectorType;
};


} // /namespace Math

} // /namespace Fork


#endif



// ========================