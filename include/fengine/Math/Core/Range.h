/*
 * Range header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RANGE_H__
#define __FORK_RANGE_H__


#include "DefaultMathTypeDefs.h"

#include <limits>
#include <algorithm>


namespace Fork
{

namespace Math
{


//! Base range class with the components: 'min' and 'max'.
template <typename T> class Range
{
    
    public:
        
        Range() :
            min{ std::numeric_limits<T>::max()    },
            max{ std::numeric_limits<T>::lowest() }
        {
        }
        Range(const T& rgMin, const T& rgMax) :
            min{ rgMin },
            max{ rgMax }
        {
        }

        /* === Functions === */

        //! Repairs this range, so that 'max' is larger than 'min'.
        Range<T>& Repair()
        {
            if (min > max)
                std::swap(min, max);
            return *this;
        }

        //! Returns the center (1D) pointer of this range.
        inline T Center() const
        {
            return (min + max) / 2;
        }

        //! Returns the range size: (max - min).
        inline T Size() const
        {
            return max - min;
        }

        //! Expands the range by inserting the specified value.
        void Insert(const T& value)
        {
            if (min > value) min = value;
            if (max < value) max = value;
        }

        //! Returns true if the specified value overlaps this range: (min <= value <= max).
        inline bool Overlap(const T& value) const
        {
            return min <= value && value <= max;
        }

        //! Returns true if this range is valid. This is case when 'min' is smaller or equal to 'max'.
        inline bool IsValid() const
        {
            return min <= max;
        }

        template <typename C> inline Range<C> Cast() const
        {
            return Range<C>(
                static_cast<C>(min),
                static_cast<C>(max)
            );
        }

        /* === Members === */

        T min = 0, max = 0;

};


/* --- Global operators --- */

template <typename T> bool operator == (const Range<T>& a, const Range<T>& b)
{
    return
        a.min == b.min &&
        a.max == b.max;
}

template <typename T> bool operator != (const Range<T>& a, const Range<T>& b)
{
    return !(a == b);
}

DEFAULT_MATH_TYPEDEFS(Range)


} // /namespace Math

} // /namespace Fork


#endif



// ========================