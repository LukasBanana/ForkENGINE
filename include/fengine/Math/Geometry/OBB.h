/*
 * OBB header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_OBB_H__
#define __FORK_OBB_H__


#include "Math/Core/DefaultMathTypeDefs.h"
#include "Math/Core/Vector2.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/Vector4.h"


namespace Fork
{

namespace Math
{


/**
Base OBB (Oriented Bounding Box) class with components: center, halfSize and axis.
\tparam V Specifies the vector type. This type must implement the following interface:
\code
static const size_t num; // Specifies the number of components.
template <typename T> T operator [] (size_t componentIndex); // Returns the specified component.
\endcode
This should be Vector2 or Vector3.
\tparam T Specifies the data type. Should be float or double.
*/
template < template <typename> class V, typename T > class OBB
{
    
    public:
        
        typedef V<T> VecT;

        OBB() = default;
        OBB(const VecT& min, const VecT& max) :
            center  { (min + max) / 2 },
            halfSize{ (max - min) / 2 }
        {
            /* Setup identity 'matrix' */
            for (size_t i = 0; i < VecT::num; ++i)
                (axis[i])[i] = T(1);
        }
        OBB(const VecT& bxCenter, const VecT& axisX, const VecT& axisY, const VecT& axisZ) :
            center  { bxCenter            },
            axis    { axisX, axisY, axisZ }
        {
            UpdateHalfSize();
        }

        /* === Functions === */

        void UpdateHalfSize()
        {
            /* Compute half size and normalize axes */
            for (size_t i = 0; i < VecT::num; ++i)
            {
                halfSize[i] = axis[i].Length();
                axis[i] *= (T(1) / halfSize[i]);
            }
        }

        //...

        /* === Members === */

        VecT    center;     //!< Box center point.
        VecT    halfSize;   //!< Box half size vector. This vector contains the half width, half height and half depth of the box.
        V<VecT> axis;       //!< Box axes vector. The y component of the x axis for instance can be accessed like this: 'axis.x.y'.

};


template <typename T> using OBB2 = OBB<Vector2, T>;
template <typename T> using OBB3 = OBB<Vector3, T>;

DEFAULT_MATH_TYPEDEFS(OBB2)
DEFAULT_MATH_TYPEDEFS(OBB3)


} // /namespace Math

} // /namespace Fork


#endif



// ========================