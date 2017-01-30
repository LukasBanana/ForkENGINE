/*
 * Tangent space mesh geometry header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TANGENT_SPACE_MESH_GEOMETRY_H__
#define __FORK_TANGENT_SPACE_MESH_GEOMETRY_H__


#include "Scene/Geometry/Node/BaseMeshGeometry.h"
#include "Video/BufferFormat/TangentSpaceVertex.h"

#include <initializer_list>


namespace Fork
{

namespace Scene
{


/**
Tangent space mesh geometry class. Uses "Video::TangentSpaceVertex" structure for vertices.
\see Video::TangentSpaceVertex
\ingroup std_geometries
*/
class FORK_EXPORT TangentSpaceMeshGeometry : public BaseMeshGeometry<Video::TangentSpaceVertex, unsigned int>
{
    
    public:
        
        //! Geometry class type.
        static const Types classType = Geometry::Types::TangentSpaceMesh;

        TangentSpaceMeshGeometry() = default;
        TangentSpaceMeshGeometry(const TangentSpaceMeshGeometry&) = delete;
        TangentSpaceMeshGeometry& operator = (const TangentSpaceMeshGeometry&) = delete;

        /* === Functions === */

        void Visit(SceneVisitor* visitor) override;

        //! Returns Geometry::Types::TangentSpaceMesh.
        Types Type() const override;

        //! Adds a new vertex. This can also be done by adding new elements to the "vertices" data member.
        void AddVertex(
            const Math::Vector3f& coord,
            const Math::Vector3f& normal = { 0, 0, 0 },
            const Math::Vector2f& texCoord = { 0, 0 }
        );

        //! Adds a new vertex. This can also be done by adding new elements to the "vertices" data member.
        void AddVertex(
            const Math::Vector3f& coord,
            const Math::Vector3f& normal,
            const Math::Vector2f& texCoord,
            const Math::Vector3f& tangent,
            const Math::Vector3f& bitangent
        );

        /**
        Generates flat tangent space vectors with the "GeometryModifier" and updates the hardware vertex buffer.
        If the hardware vertex buffer has not already been created (with the "SetupHardwareBuffer" function),
        it will be done right here.
        \see GeometryConverter::GenerateTangentSpaceFlat
        \see BaseMeshGeometry::UpdateVertexBuffer
        \see BaseMeshGeometry::SetupHardwareBuffer
        */
        void GenerateTangentSpaceFlat(bool ignoreNormals = false);

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================