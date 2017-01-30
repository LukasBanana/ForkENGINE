/*
 * ColorRGB header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_COLOR_RGB_H__
#define __FORK_COLOR_RGB_H__


#include "Math/Core/DefaultMathTypeDefs.h"
#include "Math/Core/DefaultMathOperators.h"
#include "Video/Core/MaxColorValue.h"


namespace Fork
{

namespace Video
{


/**
ColorRGB class with components r (red), g (green) and b (blue).
\tparam T Specifies the data type. Can be an integral or floating-point type.
*/
template <typename T> class ColorRGB
{
    
    public:
        
        //! Number of components: 3.
        static const size_t num = 3;

        ColorRGB() :
            r{ MaxColorValue<T>() },
            g{ MaxColorValue<T>() },
            b{ MaxColorValue<T>() }
        {
        }
        explicit ColorRGB(const T& brightness) :
            r{ brightness },
            g{ brightness },
            b{ brightness }
        {
        }
        ColorRGB(const T& red, const T& green, const T& blue) :
            r{ red   },
            g{ green },
            b{ blue  }
        {
        }

        /* === Operators === */

        ColorRGB<T>& operator += (const ColorRGB<T>& other)
        {
            r += other.r;
            g += other.g;
            b += other.b;
            return *this;
        }

        ColorRGB<T>& operator -= (const ColorRGB<T>& other)
        {
            r -= other.r;
            g -= other.g;
            b -= other.b;
            return *this;
        }

        ColorRGB<T>& operator *= (const ColorRGB<T>& other)
        {
            r *= other.r;
            g *= other.g;
            b *= other.b;
            return *this;
        }

        ColorRGB<T>& operator /= (const ColorRGB<T>& other)
        {
            r /= other.r;
            g /= other.g;
            b /= other.b;
            return *this;
        }

        ColorRGB<T>& operator *= (const T& scalar)
        {
            r *= scalar;
            g *= scalar;
            b *= scalar;
            return *this;
        }

        ColorRGB<T>& operator /= (const T& scalar)
        {
            r /= scalar;
            g /= scalar;
            b /= scalar;
            return *this;
        }

        //! Returns the additive inverse of this vector.
        inline ColorRGB<T> operator - () const
        {
            return ColorRGB<T>(-r, -g, -b);
        }

        //! Returns a constant reference to the specified component by index. Index must be 0, 1, 2 or 3.
        inline const T& operator [] (const size_t index) const
        {
            return *((&r) + index);
        }
        //! Returns a reference to the specified component by index. Index must be 0, 1, 2 or 3.
        inline T& operator [] (const size_t index)
        {
            return *((&r) + index);
        }

        /* === Functions === */

        /**
        Returns the inverse of this color.
        \see Video::InvColor
        */
        ColorRGB<T> Inv() const
        {
            return InvColor(*this);
        }

        //! Returns this color as constant raw pointer.
        inline const T* Ptr() const
        {
            return &r;
        }
        //! Returns this color as raw pointer.
        inline T* Ptr()
        {
            return &r;
        }

        /**
        Casts the ColorRGB object's data types. If the default values
        are different (see "Video::MaxColorValue"), the colors will be transformed.
        \code
        ColorRGBf colorFloat(0.5f, 1.0f, 0.0f);                       // colorFloat has the values: 0.5, 1, 0.
        ColorRGBub colorUByte(colorFloat.Cast<unsigned char, 255>()); // colorUByte has the values: 128, 255, 0.
        \endcode
        \note Don't cast float and integral colors back and forth too much!
        \see Video::MaxColorValue
        */
        template <typename C> ColorRGB<C> Cast() const
        {
            if (MaxColorValue<T>() != MaxColorValue<C>())
            {
                /* Transform color components */
                auto oldRange = static_cast<double>(MaxColorValue<T>());
                auto newRange = static_cast<double>(MaxColorValue<C>());

                auto newR = static_cast<double>(r) * newRange / oldRange;
                auto newG = static_cast<double>(g) * newRange / oldRange;
                auto newB = static_cast<double>(b) * newRange / oldRange;

                return ColorRGB<C>(
                    static_cast<C>(newR),
                    static_cast<C>(newG),
                    static_cast<C>(newB)
                );
            }

            /* Cast the color untransformed */
            return ColorRGB<C>(
                static_cast<C>(r),
                static_cast<C>(g),
                static_cast<C>(b)
            );
        }

        /* === Members === */

        T r, g, b;

};


DEFAULT_MATH_BASE_OPERATORS(ColorRGB)
DEFAULT_SCALAR_OPERATORS(ColorRGB)

typedef ColorRGB< unsigned char > ColorRGBub;
typedef ColorRGB< int           > ColorRGBi;
typedef ColorRGB< float         > ColorRGBf;
typedef ColorRGB< double        > ColorRGBd;
typedef ColorRGB< bool          > ColorRGBb;


} // /namespace Video

} // /namespace Fork


#endif



// ========================