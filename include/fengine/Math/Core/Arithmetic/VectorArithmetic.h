/*
 * Vector arithmetic header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_VECTOR_ARITHMETIC_H__
#define __FORK_VECTOR_ARITHMETIC_H__


#include "Math/Core/Equal.h"
#include "Core/StaticConfig.h"

#include <cmath>


namespace Fork
{

namespace Math
{


/**
Returns the cross product of the specified 3D vectors a and b.
\tparam T Specifies the vector type. This type must have the member varaibles x, y and z
and a constructor which takes three parameters for these coordinates.
This could be Vector3, Vector4 or your own class for a vector with three components.
\param[in] a Specifies the first vector.
\param[in] b Specifies the second vector.
\return Cross product which is of the same type as the input parameters.
\ingroup math_core
*/
template <typename T> inline T Cross(const T& a, const T& b)
{
    return T(
        a.y*b.z - b.y*a.z,
        b.x*a.z - a.x*b.z,
        a.x*b.y - b.x*a.y
    );
}

/**
Dot product main function. See more about that on the second "Dot" function.
\tparam num Specifies the number of vector components.
\tparam T Specifies the base data type.
\param[in] a Pointer to the first vector.
\param[in] b Pointer to the second vector.
\return Dot product of the two vectors a and b.
\note These pointers must never be null!
\ingroup math_core
*/
template <size_t num, typename T> inline T Dot(T const * const a, T const * const b)
{
    T p = T(0);
    
    /* Accumulate the product of all respective vector components */
    for (size_t i = 0; i < num; ++i)
        p += a[i]*b[i];
    
    return p;
}

/**
Returns the dot- (or rather scalar-) product of the specified 3D vectors a and b.
\tparam Va Specifies the vector type. This type must have a static constant field
named "num" from the type 'size_t' specifying the number of components of the vector.
It must also overload the 'operator []' for component access by index.
This could be Vector2, Vector3, Vector4 or your own class for a vector.
\tparam Vb Same rules as for Va.
\tparam T Specifies the data type. This should be a floating point type (e.g. float or double).
\param[in] a Specifies the first vector.
\param[in] b Specifies the second vector.
\note 'a' and 'b' may be from different vector types, e.g. 'a' may be from type of "Vector3"
and b may be from type of "Vector4". In this case the smallest number of components will be used,
i.e. Vector3::num (which is 3 of course).
\return Dot product which is of the same type as the input parameters.
\ingroup math_core
*/
template <
    template <typename> class Va,
    template <typename> class Vb,
    typename T >
inline T Dot(const Va<T>& a, const Vb<T>& b)
{
    return Dot<
        /* Determine lowest common number of elements */
        (Va<T>::num <= Vb<T>::num ?
            Va<T>::num :
            Vb<T>::num),
        T>(
            /* Pass vectors a and b to core "dot" function */
            &a[0], &b[0]
        );
}

/**
Vector squared length main function. See more about that on the second "LengthSq" function.
\tparam num Specifies the number of vector components.
\tparam T Specifies the base data type.
\param[in] vec Pointer to the vector.
\return Squared length of the specified vector. This is equivalent to 'Dot(vec, vec)'.
\note This pointer must never be null!
\ingroup math_core
*/
template <size_t num, typename T> inline T LengthSq(T const * const vec)
{
    return Dot<num, T>(vec, vec);
}

/**
Returns the squared length of the specified vector.
\tparam V Specifies the vector type. This must be suitable for the 'Dot' function.
\tparam T Specifies the data type. This should be a floating point type (e.g. float or double).
\param[in] vec Specifies the vector whose length is to be computed.
\return Squared length of the specified vector. This is equivalent to 'Dot(vec, vec)'.
\see Math::Dot
\ingroup math_core
*/
template < template <typename> class V, typename T > inline T LengthSq(const V<T>& vec)
{
    return LengthSq<V<T>::num, T>(&vec[0]);
}

/**
Vector length main function. See more about that on the second "Length" function.
\tparam num Specifies the number of vector components.
\tparam T Specifies the base data type.
\param[in] vec Pointer to the vector.
\return Length of the specified vector.
\note This pointer must never be null!
\ingroup math_core
*/
template <size_t num, typename T> inline T Length(T const * const vec)
{
    return std::sqrt(LengthSq<num, T>(vec));
}

/**
Returns the length of the specified vector.
\tparam V Specifies the vector type. This must be suitable for the 'Dot' function.
\tparam T Specifies the data type. This should be a floating point type (e.g. float or double).
\param[in] vec Specifies the vector whose length is to be computed.
\return Length (or rather the euclidean norm) of the specified vector.
\see Math::Dot
\ingroup math_core
*/
template < template <typename> class V, typename T > inline T Length(const V<T>& vec)
{
    return Length<V<T>::num, T>(&vec[0]);
}

/**
Returns the squared distance bewteen the two specified vectors a and b.
\tparam V Specifies the vector type. This must be suitable for the 'LengthSq'
function and must implement the member operator '-='.
\tparam T Specifies the data type. This should be a floating point type (e.g. float or double).
\param[in] a Specifies the first vector.
\param[in] b Specifies the second vector.
\return Squared distance between the two vectors. This is equivalent to the following code:
\code
LengthSq(b - a);
\endcode
\see Math::LengthSq
\ingroup math_core
*/
template < template <typename> class V, typename T > inline T DistanceSq(const V<T>& a, const V<T>& b)
{
    /* Get vector (b - a) */
    V<T> vec = b;
    vec -= a;
    
    /* Compute squared length of the vector */
    return LengthSq(vec);
}

/**
Returns the distance bewteen the two specified vectors a and b.
\tparam V Specifies the vector type. This must be suitable for the 'Length'
function and must implement the member operator '-='.
\tparam T Specifies the data type. This should be a floating point type (e.g. float or double).
\param[in] a Specifies the first vector.
\param[in] b Specifies the second vector.
\return Distance (with the euclidean norm) between the two vectors. This is equivalent to the following code:
\code
Length(b - a);
\endcode
\see Math::Length
\ingroup math_core
*/
template < template <typename> class V, typename T > inline T Distance(const V<T>& a, const V<T>& b)
{
    /* Get vector (b - a) */
    V<T> vec = b;
    vec -= a;
    
    /* Compute length of the vector */
    return Length(vec);
}

/**
Vector normalization main function. See more about that on the second "Normalize" function.
\tparam num Specifies the number of vector components.
\tparam T Specifies the base data type.
\param[in,out] vec Pointer to the vector which is to be normalized.
\return True if the vector could be normalized. Otherwise it must be a zero vector which is not normalizable.
\note This pointer must never be null!
\ingroup math_core
*/
template <size_t num, typename T> inline bool Normalize(T* const vec)
{
    /* Get the squared vector length first */
    T len = LengthSq<num, T>(vec);
    
    /* Check for zero vector */
    if (len == T(0))
        return false;
    
    /* Check if the vector is already normalized */
    if (len != T(1))
    {
        /* Compute reciprocal square root of the squared length */
        len = T(1) / std::sqrt(len);
        
        /* Normalize the vector with the inverse length for optimization */
        for (size_t i = 0; i < num; ++i)
            vec[i] *= len;
    }
    
    return true;
}

/**
Normalizes the specified vector. After that the vector has a length of 1 units.
\tparam V Specifies the vector type. This must be suitable for the 'Dot' function.
\tparam T Specifies the data type. This should be a floating point type (e.g. float or double).
\param[in,out] Vec Specifies the vector which is to be normalized.
\return True if the vector could be normalized. Otherwise it must be a zero vector which is not normalizable.
\note If the specified vector is a zero vector (i.e. x, y and z are all zero) this function has no effect.
\see Math::Dot
\ingroup math_core
*/
template < template <typename> class V, typename T > inline bool Normalize(V<T> &vec)
{
    return Normalize<V<T>::num, T>(&vec[0]);
}

/**
Vector angle main function. See more about that on the second "Angle" function.
\tparam Num Specifies the number of vector components.
\tparam T Specifies the base data type.
\param[in] a Pointer to the first vector.
\param[in] b Pointer to the second vector.
\return Angle (in radian) between the two vectors.
\note These pointers must never be null!
\ingroup math_core
*/
template <size_t num, typename T> inline T VectorAngle(T const * const a, T const * const b)
{
    return std::acos(Dot<num, T>(a, b) / (Length<num, T>(a)*Length<num, T>(b)));
}

/**
Returns the angle (in radian) bewteen the two specified 3D vectors a and b.
\tparam Va Specifies the vector type. This must be suitable for the 'Length'.
\tparam Vb Same rules as for Va.
\tparam T Specifies the data type. This should be a floating point type (e.g. float or double).
\param[in] a Specifies the first vector.
\param[in] b Specifies the second vector.
\return Angle (in radian) between the two vectors. Use the following code to get the angle in degree:
\code
VectorAngle(a, b) * Math::rad2deg;
\endcode
\see Math::Length
\ingroup math_core
*/
template <
    template <typename> class Va,
    template <typename> class Vb,
    typename T >
inline T VectorAngle(const Va<T>& a, const Vb<T>& b)
{
    return VectorAngle
        <
            /* Determine lowest common number of elements */
            (Va<T>::num <= Vb<T>::num ? Va<T>::num : Vb<T>::num),
            T
        >
        (
            /* Pass vectors a and b to main "Angle" function */
            &a[0], &b[0]
        );
}

/**
Returns the reflection vector computed by the specified incident- and surface normal vector.
\tparam V Specifies the vector type. This must be suitable for the "Dot" function.
\param[in] vec Specifies the incident vector which is to be reflected.
\param[in] normal Specifies the surface normal vector.
\return The reflected abusive vector. This is equivalent to the following code:
\code
vec - 2 * normal * Dot(vec, normal);
\endcode
\see Math::Dot
\ingroup math_core
*/
template <class V> V Reflect(const V& vec, const V& normal)
{
    return vec - V(2) * normal * Dot(vec, normal);
}

/**
Computes nearest integral value not greater than vec (for each component).
This is equivalent to the following pseudocode:
\code
{ std::floor(vec[0]), ..., std::floor(vec[T::num - 1]) }
\endcode
\tparam V Specifies the vector type. This type must have a
static constant member named "num" and must overload the 'operator []'.
*/
template <class V> V Floor(const V& vec)
{
    V result;

    for (size_t i = 0; i < V::num; ++i)
        result[i] = std::floor(vec[i]);

    return result;
}

/**
Computes nearest integral value not less than vec (for each component).
This is equivalent to the following pseudocode:
\code
{ std::ceil(vec[0]), ..., std::ceil(vec[T::num - 1]) }
\endcode
\tparam V Specifies the vector type. This type must have a
static constant member named "num" and must overload the 'operator []'.
*/
template <class V> V Ceil(const V& vec)
{
    V result;

    for (size_t i = 0; i < V::num; ++i)
        result[i] = std::ceil(vec[i]);

    return result;
}

/**
Compares the two specified vectors a and b for equalilty.
\tparam V Specifies the vector type. This class needs a static constant member called "num"
holding the number of vector components (e.g. Vector2, Vector3 or Vector4).
an integer type (e.g. int, unsigned int etc.).
\param[in] a Specifies the first vector.
\param[in] b Specifies the second vector.
\return True if the two vectors a and b are equal.
\note This function is only used as base function for the vector comparision operators (for Vector2, Vector3 and Vector4).
\see Math::Equal
\ingroup math_core
*/
template <class V> inline bool CompareVecEqual(const V& a, const V& b)
{
    for (size_t i = 0; i < V::num; ++i)
    {
        if (!Math::Equal(a[i], b[i]))
            return false;
    }
    return true;
}

/**
\see Math::CompareVecEqual
\ingroup math_core
*/
template <class V> inline bool CompareVecNotEqual(const V& a, const V& b)
{
    return !CompareVecEqual(a, b);
}

/**
\see Math::CompareVecEqual
\ingroup math_core
*/
template <class V> inline bool CompareVecLessThan(const V& a, const V& b)
{
    for (size_t i = 0; i < V::num; ++i)
    {
        if (a[i] < b[i])
            return true;
        if (a[i] > b[i])
            return false;
    }
    return false;
}

/**
\see Math::CompareVecLessThan
\ingroup math_core
*/
template <class V> inline bool CompareVecGreaterThan(const V& a, const V& b)
{
    for (size_t i = 0; i < V::num; ++i)
    {
        if (a[i] > b[i])
            return true;
        if (a[i] < b[i])
            return false;
    }
    return false;
}

/**
\see Math::CompareVecEqual
\see Math::CompareVecLessThan
\ingroup math_core
*/
template <class V> inline bool CompareVecLessThanOrEqual(const V& a, const V& b)
{
    return CompareVecLessThan(a, b) || CompareVecEqual(a, b);
}

/**
\see Math::CompareVecEqual
\see Math::CompareVecGreaterThan
\ingroup math_core
*/
template <class V> inline bool CompareVecGreaterThanOrEqual(const V& a, const V& b)
{
    return CompareVecGreaterThan(a, b) || CompareVecEqual(a, b);
}


} // /namespace Math

} // /namespace Fork


#endif



// ========================