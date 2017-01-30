/*
 * Spaces enumeration header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SPACES_H__
#define __FORK_SPACES_H__


namespace Fork
{

namespace Video
{


//! Rendering pipeline coordinates spaces.
enum class Spaces
{
    ObjectSpace,        //!< Object space for original model coordinates.
    WorldSpace,         //!< World space.
    ViewSpace,          //!< View space (Inverse of the camera matrix).
    ProjectionSpace,    //!< Projection space before dividing by W.
    /**
    Clipping space for normalized device coordinates (NDC) after projection and dividing by W.
    In Direct3D the coordinate ranges are { [-1 .. 1], [-1 .. 1], [0 .. 1] }.
    In OpenGL the coordinate ranges are { [-1 .. 1], [-1 .. 1], [-1 .. 1] }.
    */
    ClippingSpace,
    ScreenSpace,        //!< Homogeneous screen space after viewport transformation.
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================