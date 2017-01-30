/*
 * Editor wxWidgets vector3 header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_WX_VECTOR3_H__
#define __FORK_EDITOR_WX_VECTOR3_H__


#include "Math/Core/Vector3.h"

#include <wx/wx.h>
#include <wx/propgrid/property.h>


//! wxWidgets vector3 extension class.
class WxVector3
{
    
    public:
        
        static const size_t num = 4;

        WxVector3() = default;
        WxVector3(double vX, double vY, double vZ) :
            x(vX),
            y(vY),
            z(vZ)
        {
        }
        WxVector3(const Fork::Math::Vector3f& vec) :
            x(vec.x),
            y(vec.y),
            z(vec.z)
        {
        }

        inline operator Fork::Math::Vector3f() const
        {
            return Fork::Math::Vector3d(x, y, z).Cast<float>();
        }

        inline float operator [] (const size_t index) const
        {
            return *((&x) + index);
        }

        double x = 0.0, y = 0.0, z = 0.0;

};


inline bool operator == (const WxVector3& a, const WxVector3& b)
{
    return Fork::Math::CompareVecEqual(a, b);
}


WX_PG_DECLARE_VARIANT_DATA(WxVector3)


#endif



// ========================