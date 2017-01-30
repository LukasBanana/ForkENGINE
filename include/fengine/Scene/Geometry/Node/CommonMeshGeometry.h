/*
 * Common mesh geometry header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_COMMON_MESH_GEOMETRY_H__
#define __FORK_COMMON_MESH_GEOMETRY_H__


#include "Scene/Geometry/Node/BaseMeshGeometry.h"
#include "Video/BufferFormat/CommonVertex.h"

#include <initializer_list>


namespace Fork
{

namespace Scene
{


/**
Common mesh geometry class. Uses "Video::CommonVertex" structure for vertices.
\see Video::CommonVertex
\ingroup std_geometries
*/
class FORK_EXPORT CommonMeshGeometry : public BaseMeshGeometry<Video::CommonVertex, unsigned int>
{
    
    public:
        
        //! Geometry class type.
        static const Types classType = Geometry::Types::CommonMesh;

        CommonMeshGeometry() = default;
        CommonMeshGeometry(const CommonMeshGeometry&) = delete;
        CommonMeshGeometry& operator = (const CommonMeshGeometry&) = delete;

        /* === Functions === */

        void Visit(SceneVisitor* visitor) override;

        //! Returns Geometry::Types::CommonMesh.
        Types Type() const override;

        //! Adds a new vertex. This can also be done by adding new elements to the "vertices" data member.
        void AddVertex(
            const Math::Vector3f& coord,
            const Math::Vector3f& normal = { 0, 0, 0 },
            const Video::ColorRGBAf& color = Video::ColorRGBAf()
        );

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================