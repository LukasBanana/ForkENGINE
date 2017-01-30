/*
 * Billboard geometry header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_BILLBOARD_GEOMETRY_H__
#define __FORK_BILLBOARD_GEOMETRY_H__


#include "Scene/Geometry/Node/Geometry.h"


namespace Fork
{

namespace Scene
{


/**
Billboard geometry class. A billboard is a quad (two triangles) which is always
faced to the view camera. This can be used for sprite or particle effects.
When rendering thousands of particles, you should use a spcific particle effect provider,
and thousands of "BillboardGeometry" instances!
\ingroup std_geometries
*/
class FORK_EXPORT BillboardGeometry : public Geometry
{
    
    public:
        
        //! Geometry class type.
        static const Types classType = Geometry::Types::Billboard;

        BillboardGeometry() = default;
        virtual ~BillboardGeometry();

        BillboardGeometry(const BillboardGeometry&) = delete;
        BillboardGeometry& operator = (const BillboardGeometry&) = delete;

        /* === Functions === */

        void Visit(SceneVisitor* visitor) override;

        //! Returns Geometry::Types::Billboard.
        Types Type() const override;

        //! Computes the bounding volume of this mesh geometry.
        void ComputeBoundingVolume() override;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================