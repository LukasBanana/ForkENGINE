/*
 * Raster number header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_COMMON_RASTER_NUMBER_H__
#define __FORK_MATH_COMMON_RASTER_NUMBER_H__


#include "Math/Core/DefaultMathOperators.h"

#include <cmath>


namespace Fork
{

namespace Math
{


/**
This class only stores a single number which can be increased (or decreased) in a dynamic raster.
\tparam T Specifies the data type. This should be float or double. By default float.
*/
template <typename T = float> class RasterNumber
{
    
    public:
        
        RasterNumber() = default;
        RasterNumber(const T& number) :
            number_{ number }
        {
        }

        inline RasterNumber<T>& operator += (const RasterNumber<T>& other)
        {
            number_ += other.number_;
            return *this;
        }
        inline RasterNumber<T>& operator -= (const RasterNumber<T>& other)
        {
            number_ -= other.number_;
            return *this;
        }
        inline RasterNumber<T>& operator *= (const RasterNumber<T>& other)
        {
            number_ *= other.number_;
            return *this;
        }
        inline RasterNumber<T>& operator /= (const RasterNumber<T>& other)
        {
            number_ /= other.number_;
            return *this;
        }

        /**
        Returns the number inside the specified raster size.
        \param[in] rasterSize Specifies the raster size.
        \remarks Example:
        \code
        RasterNumber( 3.5f).Raster(5.0f) ==  5.0f
        RasterNumber( 2.4f).Raster(5.0f) ==  0.0f
        RasterNumber(-4.9f).Raster(5.0f) == -5.0f
        RasterNumber( 8.3f).Raster(5.0f) == 10.0f
        RasterNumber( 3.5f).Raster(3.0f) ==  3.0f
        RasterNumber( 1.5f).Raster(3.0f) ==  3.0f
        RasterNumber(-1.4f).Raster(3.0f) ==  0.0f
        RasterNumber(-1.7f).Raster(3.0f) == -3.0f
        \endcode
        */
        T Raster(const T& rasterSize) const
        {
            /* Scale inverse, round and scale back */
            return std::round(number_ / rasterSize) * rasterSize;
        }

        /**
        Returns the number inside the specified raster size and flushes the
        internal number if the raster value is not zero.
        \remarks Example:
        \code
        RasterNumber<float> num;
        num +=  2.3f; num.RasterFlush(5.0f) ==  0.0f; // Internal number is now 2.3
        num +=  1.0f; num.RasterFlush(5.0f) ==  5.0f; // Internal number is now 3.3 - 5 = -1.7
        num +=  1.0f; num.RasterFlush(5.0f) ==  0.0f; // Internal number is now -0.7
        num += 10.5f; num.RasterFlush(5.0f) == 10.0f; // Internal number is now 9.8 - 10 = -0.2
        \endcode
        \see ToNum
        */
        T RasterFlush(const T& rasterSize)
        {
            /* Scaled (S) and rounded (R) number */
            auto srNum = std::round(number_ / rasterSize);

            /* Check if raster is non-zero */
            if (std::abs(srNum) > T(0.5))
            {
                /* Scale number back */
                srNum *= rasterSize;

                /*
                Store final raster number temporarily
                and reduce internal number by the result
                */
                const auto rasterNum = srNum;
                number_ -= rasterNum;

                return rasterNum;
            }

            return T(0);
        }

        /**
        Simple cast operator. This will not apply any raster! To make use of a raster use "Raster" or "RasterFlush".
        \see Raster
        \see RasterFlush
        */
        inline operator T () const
        {
            return number_;
        }

    private:
        
        T number_ = T(0);

};


DEFAULT_MATH_BASE_OPERATORS(RasterNumber)


} // /namespace Math

} // /namespace Fork


#endif



// ========================