/*
 * Bouding volume header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_BOUNDING_VOLUME_H__
#define __FORK_BOUNDING_VOLUME_H__


#include "Core/Export.h"
#include "Math/Geometry/AABB.h"
#include "Math/Geometry/Sphere.h"


namespace Fork
{

namespace Scene
{


//! Bounding volume data model.
class FORK_EXPORT BoundingVolume
{
    
    public:
        
        //! Bounding volume model types.
        enum class Types
        {
            None,   //!< No bounding volume used.
            Box,    //!< AABB (Axis-Aligned Bounding Box).
            Sphere  //!< Sphere with radius.
        };

        /**
        Determines the active bounding volume type.
        This function will set the "type" member to that with the smallest volume.
        \remarks The bounding box and bounding sphere must be computed first.
        At initialization they are both empty.
        \see Math::AABB::Volume
        \see Math::Sphere::Volume
        */
        void DetermineType();

        //! Sets the bounding box, sets 'type' to Types::Box and updates the bounding sphere.
        void SetupBox(const Math::AABB3f& boundingBox);
        //! Sets the bounding sphere, sets 'type' to Types::Sphere and updates the bounding box.
        void SetupSphere(const Math::Sphere<>& boundingSphere);

        /**
        Active bounding volume type. By default Types::None.
        \remarks Note that testing a sphere against a view frustum is much faster than testing a bounding box.
        \see DetermineType
        \see Types
        */
        Types type = Types::None;

        /**
        Bounding box. This can be manipulated to improve performance,
        e.g. for a cube the size can be computed faster than iterating over all vertices.
        \see Math::AABB
        */
        Math::AABB3f box;
        /**
        Bounding sphere.
        \see Math::Sphere
        */
        Math::Sphere<> sphere;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================