/*
 * Terrain mesh geometry header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TERRAIN_MESH_GEOMETRY_H__
#define __FORK_TERRAIN_MESH_GEOMETRY_H__


#include "Scene/Geometry/Node/BaseMeshGeometry.h"
#include "Video/BufferFormat/TerrainVertex.h"

#include <initializer_list>
#include <array>


namespace Fork
{

namespace Scene
{


/**
Terrain mesh geometry class. Uses "Video::TerrainVertex" structure for vertices.
Such a geometry is used as 'geometric template' to construct a terrain.
\see Video::TerrainVertex
\ingroup std_geometries
*/
class FORK_EXPORT TerrainMeshGeometry : public BaseMeshGeometry<Video::TerrainVertex, unsigned int>
{
    
    public:
        
        //! Geometry class type.
        static const Types classType = Geometry::Types::TerrainMesh;

        /* === Enumerations === */

        //! Grid chunk types enumeration.
        enum class GridChunkTypes
        {
            Base = 0,           //!< Grid base.
            
            LeftEdge,           //!< Grid left edge.
            RightEdge,          //!< Grid right edge.
            TopEdge,            //!< Grid top edge.
            BottomEdge,         //!< Grid bottom edge.

            LeftTopCorner,      //!< Grid left-top corner.
            LeftBottomCorner,   //!< Grid left-bottom corner.
            RightTopCorner,     //!< Grid right-top corner.
            RightBottomCorner,  //!< Grid right-bottom corner.
        };

        /* === Structures === */

        /**
        This structure stores the index offset and number of
        vertices to render for a respective terrain chunk.
        */
        struct GridChunk
        {
            unsigned int numVertices = 0; //!< Number of vertices to render. By default 0.
            unsigned int firstVertex = 0; //!< First vertex index. By default 0.
        };

        //! Array type of the grid chunks.
        typedef std::array<GridChunk, 9> GridChunkArray;

        /* === Functions === */

        TerrainMeshGeometry() = default;
        TerrainMeshGeometry(const TerrainMeshGeometry&) = delete;
        TerrainMeshGeometry& operator = (const TerrainMeshGeometry&) = delete;

        void Visit(SceneVisitor* visitor) override;

        //! Returns Geometry::Types::TerrainMesh.
        Types Type() const override;

        //! Adds a new vertex. This can also be done by adding new elements to the "vertices" data member.
        void AddVertex(const Math::Point2f& coord);

        /**
        Returns a reference to the grid chunk by the specified type.
        \throws IndexOutOfBoundsException (Only in debug mode) When 'type' is out of bounds.
        */
        GridChunk& GetGridChunk(const GridChunkTypes type);
        /**
        Returns a constant reference to the grid chunk by the specified type.
        \throws IndexOutOfBoundsException (Only in debug mode) When 'type' is out of bounds.
        */
        const GridChunk& GetGridChunk(const GridChunkTypes type) const;

        /**
        Specifies the grid chunk array. These elements contains the vertex
        index offsets to render the respective terrain grid chunk.
        Use "GridChunkTypes" to access the array elements.
        \see GridChunkTypes
        */
        GridChunkArray gridChunks;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================