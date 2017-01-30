/*
 * Editor wxWidgets4 vector header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_WX_VECTOR4_H__
#define __FORK_EDITOR_WX_VECTOR4_H__


#include "Math/Core/Vector4.h"

#include <wx/wx.h>
#include <wx/propgrid/property.h>


//! wxWidgets vector4 extension class.
class WxVector4
{
    
    public:
        
        static const size_t num = 4;

        WxVector4() = default;
        WxVector4(double vX, double vY, double vZ, double vW = 1.0) :
            x(vX),
            y(vY),
            z(vZ),
            w(vW)
        {
        }
        WxVector4(const Fork::Math::Vector4f& vec) :
            x(vec.x),
            y(vec.y),
            z(vec.z),
            w(vec.w)
        {
        }

        inline operator Fork::Math::Vector4f() const
        {
            return Fork::Math::Vector4d(x, y, z, w).Cast<float>();
        }

        inline double operator [] (const size_t index) const
        {
            return *((&x) + index);
        }

        double x = 0.0, y = 0.0, z = 0.0, w = 1.0;

};


inline bool operator == (const WxVector4& a, const WxVector4& b)
{
    return Fork::Math::CompareVecEqual(a, b);
}


WX_PG_DECLARE_VARIANT_DATA(WxVector4)


#endif



// ========================