/*
 * Terrain geometry header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TERRAIN_GEOMETRY_H__
#define __FORK_TERRAIN_GEOMETRY_H__


#include "Scene/Geometry/Node/Geometry.h"
#include "Scene/Geometry/Node/TerrainMeshGeometry.h"


namespace Fork
{

namespace Scene
{


/**
Terrain geometry class. This should always be a child node (or children's child node)
of a "TexturedGeometry", because the terrain needs at least a single height field texture,
which is used by the respective vertex shader to transform the terrain vertices.
\see TerrainMeshGeometry
\ingroup std_geometries
*/
class FORK_EXPORT TerrainGeometry : public Geometry
{
    
    public:
        
        //! Geometry class type.
        static const Types classType = Geometry::Types::Terrain;

        TerrainGeometry() = default;
        virtual ~TerrainGeometry();

        TerrainGeometry(const TerrainGeometry&) = delete;
        TerrainGeometry& operator = (const TerrainGeometry&) = delete;

        /* === Functions === */

        void Visit(SceneVisitor* visitor) override;

        //! Returns Geometry::Types::Terrain.
        Types Type() const override;

        /**
        Copies the bounding volume of the actual geometry.
        \todo Incomplete (bounding volume works differently for terrains!).
        */
        void ComputeBoundingVolume() override;

        /* === Members === */

        /**
        This is the terrain template geometry. This will be rendered several times for a single terrain
        and the respective vertex shader will transform the vertices to its height field.
        \see TerrainMeshGeometry
        */
        TerrainMeshGeometryPtr templateGeometry;

        /**
        Specifies the number of geometry MIP levels to render the terrain.
        If this is 0, no geometry will be rendered.
        If this is 1, only a single geometry LOD (level-of-detail) will be rendered for the entire terrain.
        If this is 2+, several geometry MIP levels will be rendered depending on the view camera distance to the terrain.
        By default 5.
        */
        unsigned short geoMIPLevels = 5;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================