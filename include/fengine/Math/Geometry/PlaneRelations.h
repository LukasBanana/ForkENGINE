/*
 * Plane relation enumerations header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PLANE_RELATIONS_H__
#define __FORK_PLANE_RELATIONS_H__


namespace Fork
{

namespace Math
{


//! Relations between plane and AABB.
enum class PlaneAABBRelations
{
    InFrontOf,  //!< Box is in the front of the plane.
    Behind,     //!< Box is in the behind the plane.
    Clipped,    //!< Box is clipped by the plane.
};

//! Relations between plane and point.
enum class PlanePointRelations
{
    InFrontOf,  //!< Point is in the front of the plane.
    Behind,     //!< Point is in the behind the plane.
    Onto,       //!< Point is onto the plane.
};


} // /namespace Math

} // /namespace Fork


#endif



// ========================