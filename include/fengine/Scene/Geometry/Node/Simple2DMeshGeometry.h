/*
 * Simple 2D mesh geometry header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SIMPLE_2D_MESH_GEOMETRY_H__
#define __FORK_SIMPLE_2D_MESH_GEOMETRY_H__


#include "Scene/Geometry/Node/BaseMeshGeometry.h"
#include "Video/BufferFormat/Simple2DVertex.h"

#include <initializer_list>


namespace Fork
{

namespace Scene
{


/**
Simple 2D mesh geometry class. Uses "Video::Simple2DVertex" structure for vertices.
\see Video::Simple2DVertex
\ingroup std_geometries
*/
class FORK_EXPORT Simple2DMeshGeometry : public BaseMeshGeometry<Video::Simple2DVertex, unsigned int>
{
    
    public:
        
        //! Geometry class type.
        static const Types classType = Geometry::Types::Simple2DMesh;

        Simple2DMeshGeometry() = default;
        Simple2DMeshGeometry(const Simple2DMeshGeometry&) = delete;
        Simple2DMeshGeometry& operator = (const Simple2DMeshGeometry&) = delete;

        /* === Functions === */

        void Visit(SceneVisitor* visitor) override;

        //! Returns Geometry::Types::Simple2DMesh.
        Types Type() const override;

        //! Adds a new vertex. This can also be done by adding new elements to the "vertices" data member.
        void AddVertex(const Math::Point2f& coord, const Math::Vector2f& texCoord = { 0, 0 });

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================