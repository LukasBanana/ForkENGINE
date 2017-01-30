/*
 * ColorRGBA header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_COLOR_RGBA_H__
#define __FORK_COLOR_RGBA_H__


#include "Math/Core/DefaultMathTypeDefs.h"
#include "Math/Core/DefaultMathOperators.h"
#include "Video/Core/MaxColorValue.h"
#include "Video/Core/ColorRGB.h"


namespace Fork
{

namespace Video
{


/**
ColorRGBA class with components r (red), g (green), b (blue) and a (alpha).
\tparam T Specifies the data type. Can be an integral or floating-point type.
*/
template <typename T> class ColorRGBA
{
    
    public:
        
        //! Number of components: 4.
        static const size_t num = 4;

        ColorRGBA() :
            r{ MaxColorValue<T>() },
            g{ MaxColorValue<T>() },
            b{ MaxColorValue<T>() },
            a{ MaxColorValue<T>() }
        {
        }
        explicit ColorRGBA(const T& brightness) :
            r{ brightness         },
            g{ brightness         },
            b{ brightness         },
            a{ MaxColorValue<T>() }
        {
        }
        ColorRGBA(const T& red, const T& green, const T& blue) :
            r{ red                },
            g{ green              },
            b{ blue               },
            a{ MaxColorValue<T>() }
        {
        }
        ColorRGBA(const T& red, const T& green, const T& blue, const T& alpha) :
            r{ red   },
            g{ green },
            b{ blue  },
            a{ alpha }
        {
        }
        /**
        This constructor is not explicit, since when a function expects a ColorRGBA object,
        you should be able to use just a ColorRGB object to set the alpha channel implicit.
        */
        ColorRGBA(const ColorRGB<T>& other) :
            r{ other.r            },
            g{ other.g            },
            b{ other.b            },
            a{ MaxColorValue<T>() }
        {
        }
        ColorRGBA(const ColorRGB<T>& other, const T& alpha) :
            r{ other.r },
            g{ other.g },
            b{ other.b },
            a{ alpha   }
        {
        }

        /* === Operators === */

        ColorRGBA<T>& operator += (const ColorRGBA<T>& other)
        {
            r += other.r;
            g += other.g;
            b += other.b;
            a += other.a;
            return *this;
        }

        ColorRGBA<T>& operator -= (const ColorRGBA<T>& other)
        {
            r -= other.r;
            g -= other.g;
            b -= other.b;
            a -= other.a;
            return *this;
        }

        ColorRGBA<T>& operator *= (const ColorRGBA<T>& other)
        {
            r *= other.r;
            g *= other.g;
            b *= other.b;
            a *= other.a;
            return *this;
        }

        ColorRGBA<T>& operator /= (const ColorRGBA<T>& other)
        {
            r /= other.r;
            g /= other.g;
            b /= other.b;
            a /= other.a;
            return *this;
        }

        ColorRGBA<T>& operator *= (const T& scalar)
        {
            r *= scalar;
            g *= scalar;
            b *= scalar;
            a *= scalar;
            return *this;
        }

        ColorRGBA<T>& operator /= (const T& scalar)
        {
            r /= scalar;
            g /= scalar;
            b /= scalar;
            a /= scalar;
            return *this;
        }

        //! Returns the additive inverse of this vector.
        inline ColorRGBA<T> operator - () const
        {
            return ColorRGBA<T>(-r, -g, -b, -a);
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

        //! Converts this RGBA color into an RGB color.
        inline ColorRGB<T> ToRGB() const
        {
            return { r, g, b };
        }

        /**
        Returns the inverse of this color.
        \see Video::InvColor
        */
        ColorRGBA<T> Inv() const
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
        Casts the ColorRGBA object's data types. If the maximal color values
        are different (see "Video::MaxColorValue"), the colors will be transformed.
        \code
        ColorRGBAf colorFloat(0.5f, 1.0f, 0.0f, 1.0f);                 // colorFloat has the values: 0.5, 1, 0, 1.
        ColorRGBAub colorUByte(colorFloat.Cast<unsigned char, 255>()); // colorUByte has the values: 128, 255, 0, 255.
        \endcode
        \note Don't cast float and integral colors back and forth too much!
        \see Video::MaxColorValue
        */
        template <typename C> inline ColorRGBA<C> Cast() const
        {
            if (MaxColorValue<T>() != MaxColorValue<C>())
            {
                /* Transform color components */
                auto oldRange = static_cast<double>(MaxColorValue<T>());
                auto newRange = static_cast<double>(MaxColorValue<C>());

                auto newR = static_cast<double>(r) * newRange / oldRange;
                auto newG = static_cast<double>(g) * newRange / oldRange;
                auto newB = static_cast<double>(b) * newRange / oldRange;
                auto newA = static_cast<double>(a) * newRange / oldRange;

                return ColorRGBA<C>(
                    static_cast<C>(newR),
                    static_cast<C>(newG),
                    static_cast<C>(newB),
                    static_cast<C>(newA)
                );
            }

            /* Cast the color untransformed */
            return ColorRGBA<C>(
                static_cast<C>(r),
                static_cast<C>(g),
                static_cast<C>(b),
                static_cast<C>(a)
            );
        }

        /* === Members === */

        T r, g, b, a;

};


DEFAULT_MATH_BASE_OPERATORS(ColorRGBA)
DEFAULT_SCALAR_OPERATORS(ColorRGBA)

typedef ColorRGBA< unsigned char > ColorRGBAub;
typedef ColorRGBA< int           > ColorRGBAi;
typedef ColorRGBA< float         > ColorRGBAf;
typedef ColorRGBA< double        > ColorRGBAd;
typedef ColorRGBA< bool          > ColorRGBAb;


} // /namespace Video

} // /namespace Fork


#endif



// ========================