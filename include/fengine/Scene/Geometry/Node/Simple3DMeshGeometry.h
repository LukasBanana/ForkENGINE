/*
 * Simple 3D mesh geometry header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SIMPLE_3D_MESH_GEOMETRY_H__
#define __FORK_SIMPLE_3D_MESH_GEOMETRY_H__


#include "Scene/Geometry/Node/BaseMeshGeometry.h"
#include "Video/BufferFormat/Simple3DVertex.h"


namespace Fork
{

namespace Scene
{


/**
Simple 3D mesh geometry class. Uses "Video::Simple3DVertex" structure for vertices.
\see Video::Simple3DVertex
\ingroup std_geometries
*/
class FORK_EXPORT Simple3DMeshGeometry : public BaseMeshGeometry<Video::Simple3DVertex, unsigned int>
{
    
    public:
        
        //! Geometry class type.
        static const Types classType = Geometry::Types::Simple3DMesh;

        Simple3DMeshGeometry() = default;
        Simple3DMeshGeometry(const Simple3DMeshGeometry&) = delete;
        Simple3DMeshGeometry& operator = (const Simple3DMeshGeometry&) = delete;

        /* === Functions === */

        void Visit(SceneVisitor* visitor) override;

        //! Returns Geometry::Types::Simple3DMesh.
        Types Type() const override;

        //! Adds a new vertex. This can also be done by adding new elements to the "vertices" data member.
        void AddVertex(
            const Math::Point3f& coord,
            const Math::Vector3f& normal = { 0, 0, 0 },
            const Math::Vector2f& texCoord = { 0, 0 }
        );

        /**
        Generates flat normals with the "GeometryModifier" and updates the hardware vertex buffer.
        If the hardware vertex buffer has not already been created (with the "SetupHardwareBuffer" function),
        it will be done right here.
        \see GeometryConverter::GenerateNormalsFlat
        \see BaseMeshGeometry::UpdateVertexBuffer
        \see BaseMeshGeometry::SetupHardwareBuffer
        */
        void GenerateNormalsFlat();

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================