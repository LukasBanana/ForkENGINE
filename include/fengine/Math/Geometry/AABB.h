/*
 * AABB header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_AABB_H__
#define __FORK_AABB_H__


#include "Math/Core/DefaultMathTypeDefs.h"
#include "Math/Core/Vector2.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/VectorSizeCounterparts.h"

#include <limits>


namespace Fork
{

namespace Math
{


/**
Base AABB (Axis Aligned Bounding Box) class with components: "min" and "max".
\tparam V Specifies the vector type. This should be Vector2 or Vector3.
\tparam T Specifies the data type. This should be float or double.
\remarks Because this AABB class has an abstract dimension, the functions
to compute its volume and area are separated to global math functions
(see "Math::ComputeAABBVolume" and "Math::ComputeAABBArea").
\see Math::ComputeAABBVolume
\see Math::ComputeAABBArea
*/
template < template <typename> class V, typename T > class AABB
{
    
    public:
        
        typedef V<T> VecT;
        typedef typename VectorCounterpart<V, T>::SizeType SizeT;

        AABB() :
            min{ VecT(std::numeric_limits<T>::max())    },
            max{ VecT(std::numeric_limits<T>::lowest()) }
        {
        }
        AABB(const VecT& boxMin, const VecT& boxMax) :
            min{ boxMin },
            max{ boxMax }
        {
        }

        /* === Functions === */

        /**
        Returns true if this is a valid bounding box.
        This is the case, when each component of the "min" member is smaller
        or equal to the respective component of the "max" member.
        */
        bool IsValid() const
        {
            for (size_t i = 0; i < VecT::num; ++i)
            {
                if (min[i] > max[i])
                    return false;
            }
            return true;
        }

        AABB<V, T>& Repair()
        {
            for (size_t i = 0; i < VecT::num; ++i)
            {
                if (min[i] > max[i])
                    std::swap(min[i], max[i]);
            }
            return *this;
        }

        /**
        Invalidates this box. This is the default configuration.
        \see IsValid
        */
        AABB<V, T>& Invalidate()
        {
            min = VecT(std::numeric_limits<T>::max());
            max = VecT(std::numeric_limits<T>::lowest());
            return *this;
        }

        //! Returns the center of this box.
        inline VecT Center() const
        {
            return (min + max) / T(2);
        }

        //! Returns the box size. 'SizeT' is either Size2<T> or Size3<T>.
        inline SizeT Size() const
        {
            return SizeT(max - min);
        }

        //! Expands the box by inserting the specified point.
        void InsertPoint(const VecT& point)
        {
            for (size_t i = 0; i < VecT::num; ++i)
            {
                if (point[i] < min[i])
                    min[i] = point[i];
                if (point[i] > max[i])
                    max[i] = point[i];
            }
        }

        //! Expands the box by inserting the specified box (if 'other' is a valid box).
        void InsertBox(const AABB<V, T>& other)
        {
            if (other.IsValid())
            {
                InsertPoint(other.min);
                InsertPoint(other.max);
            }
        }

        /* === Members === */

        VecT min, max;

};


template <typename T> using AABB2 = AABB<Vector2, T>;
template <typename T> using AABB3 = AABB<Vector3, T>;

DEFAULT_MATH_TYPEDEFS(AABB2)
DEFAULT_MATH_TYPEDEFS(AABB3)


} // /namespace Math

} // /namespace Fork


#endif



// ========================