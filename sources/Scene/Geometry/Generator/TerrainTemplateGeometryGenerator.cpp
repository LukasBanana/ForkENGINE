/*
 * Terrain template geometry generator header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Generator/GeometryGenerator.h"
#include "Core/Exception/InvalidArgumentException.h"


namespace Fork
{

namespace Scene
{

namespace GeometryGenerator
{


/*
Generates the terrain template. This will be a vertex buffer with several chunks
inside a single mesh geometry: 1 Base Chunk, 4 Edge Chunks and 4 Corner Chunks.
*/
FORK_EXPORT void GenerateTerrainTemplate(TerrainMeshGeometry& geometry, unsigned int gridSize)
{
    if (gridSize == 0)
        throw InvalidArgumentException(__FUNCTION__, "gridSize", "Parameter must not be 0");

    /* Temporaries for mesh construction */
    unsigned int indexOffset = 0;
    const float invGridSize = 1.0f / gridSize;

    size_t vertexIndex = 0;
    std::array<Math::Point2f, 6> tempVertices;

    typedef Math::Point2ui GridPoint;
    typedef TerrainMeshGeometry::GridChunkTypes ChunkTypes;
    typedef TerrainMeshGeometry::GridChunk Chunk;

    /* --- Internal functions --- */

    /* Adds a single vertex to the temporary vertex list */
    auto AddVertex = [&](unsigned int u, unsigned int v)
    {
        if (vertexIndex < 6)
        {
            tempVertices[vertexIndex].x = invGridSize * static_cast<float>(u) * 0.5f;
            tempVertices[vertexIndex].y = invGridSize * static_cast<float>(v) * 0.5f;
            ++vertexIndex;
        }
    };

    /* Adds a triangle out of three vertices from the temporary vertex list */
    auto AddTriangle = [&](unsigned int v0, unsigned int v1, unsigned int v2)
    {
        geometry.AddVertex(tempVertices[v0]);
        geometry.AddVertex(tempVertices[v1]);
        geometry.AddVertex(tempVertices[v2]);
    };

    /*
    Creates the vertices for a quad:
    X-------------X
    |           / |
    |         /   |
    |       /     |
    |     /       |
    |   /         |
    | /           |
    X-------------X
    */
    auto CreateQuad = [&](unsigned int u, unsigned v)
    {
        AddVertex(u    , v    );
        AddVertex(u    , v + 2);
        AddVertex(u + 2, v + 2);
        AddVertex(u + 2, v    );

        AddTriangle(0, 1, 2);
        AddTriangle(0, 2, 3);

        vertexIndex = 0;
    };

    /*
    Creates the vertices for a single edge:
    X-------------X
    |      __-- / |
    |  __--   /   |
    X--     /     | Example: Left Edge
    |     /       |
    |   /         |
    | /           |
    X-------------X
    */
    auto CreateEdge = [&](unsigned int u, unsigned int v, const ChunkTypes chunkType)
    {
        switch (chunkType)
        {
            case ChunkTypes::LeftEdge:
                AddVertex(u    , v + 2);
                AddVertex(u + 2, v + 2);
                AddVertex(u    , v + 1);
                AddVertex(u + 2, v    );
                AddVertex(u    , v    );
                break;

            case ChunkTypes::RightEdge:
                AddVertex(u + 2, v    );
                AddVertex(u    , v    );
                AddVertex(u + 2, v + 1);
                AddVertex(u    , v + 2);
                AddVertex(u + 2, v + 2);
                break;

            case ChunkTypes::TopEdge:
                AddVertex(u + 2, v + 2);
                AddVertex(u + 2, v    );
                AddVertex(u + 1, v + 2);
                AddVertex(u    , v    );
                AddVertex(u    , v + 2);
                break;

            case ChunkTypes::BottomEdge:
                AddVertex(u    , v    );
                AddVertex(u    , v + 2);
                AddVertex(u + 1, v    );
                AddVertex(u + 2, v + 2);
                AddVertex(u + 2, v    );
                break;

            default:
                return;
        }

        AddTriangle(0, 1, 2);
        AddTriangle(2, 1, 3);
        AddTriangle(3, 4, 2);

        vertexIndex = 0;
    };

    /*
    Creates the vertices for a single corner:
    X-------------X
    |      __-- / |
    |  __--   / / |
    X--     /  /  | Example: Left-Bottom Corner
    |     /   /   |
    |   /    /    |
    | /     /     |
    X------X------X
    */
    auto CreateCorner = [&](unsigned int u, unsigned int v, const ChunkTypes chunkType)
    {
        switch (chunkType)
        {
            case ChunkTypes::LeftTopCorner:
                AddVertex(u + 2, v    );
                AddVertex(u    , v + 1);
                AddVertex(u + 1, v + 2);
                AddVertex(u    , v    );
                AddVertex(u    , v + 2);
                AddVertex(u + 2, v + 2);
                break;

            case ChunkTypes::LeftBottomCorner:
                AddVertex(u + 2, v + 2);
                AddVertex(u + 1, v    );
                AddVertex(u    , v + 1);
                AddVertex(u + 2, v    );
                AddVertex(u    , v    );
                AddVertex(u    , v + 2);
                break;

            case ChunkTypes::RightTopCorner:
                AddVertex(u    , v    );
                AddVertex(u + 1, v + 2);
                AddVertex(u + 2, v + 1);
                AddVertex(u    , v + 2);
                AddVertex(u + 2, v + 2);
                AddVertex(u + 2, v    );
                break;

            case ChunkTypes::RightBottomCorner:
                AddVertex(u    , v + 2);
                AddVertex(u + 2, v + 1);
                AddVertex(u + 1, v    );
                AddVertex(u + 2, v + 2);
                AddVertex(u + 2, v    );
                AddVertex(u    , v    );
                break;

            default:
                return;
        }

        AddTriangle(0, 1, 2);
        AddTriangle(0, 3, 1);
        AddTriangle(1, 4, 2);
        AddTriangle(0, 2, 5);

        vertexIndex = 0;
    };

    /*
    Converts the specified corner chunk type to an edge type,
    depending on the vertex position.
    */
    auto CornerTypeToEdgeType = [&](const GridPoint& pos, const ChunkTypes cornerType) -> ChunkTypes
    {
        switch (cornerType)
        {
            case ChunkTypes::LeftTopCorner:
                return pos.x == 0 ? ChunkTypes::LeftEdge : ChunkTypes::TopEdge;
            case ChunkTypes::LeftBottomCorner:
                return pos.x == 0 ? ChunkTypes::LeftEdge : ChunkTypes::BottomEdge;
            case ChunkTypes::RightTopCorner:
                return pos.x + 1 == gridSize ? ChunkTypes::RightEdge : ChunkTypes::TopEdge;
            case ChunkTypes::RightBottomCorner:
                return pos.x + 1 == gridSize ? ChunkTypes::RightEdge : ChunkTypes::BottomEdge;
            default:
                break;
        }
        return ChunkTypes::Base;
    };

    /* Returns true if the specified position is at an edge of the grid boundary */
    auto IsPosAtEdge = [&](const GridPoint& pos, const ChunkTypes edgeType) -> bool
    {
        switch (edgeType)
        {
            case ChunkTypes::LeftEdge:
                return pos.x == 0;
            case ChunkTypes::RightEdge:
                return pos.x + 1 == gridSize;
            case ChunkTypes::TopEdge:
                return pos.y + 1 == gridSize;
            case ChunkTypes::BottomEdge:
                return pos.y == 0;
            default:
                break;
        }
        return false;
    };

    /* Returns true if the specified position is at a corner of the grid boundary */
    auto IsPosAtCorner = [&](const GridPoint& pos, const ChunkTypes cornerType) -> bool
    {
        switch (cornerType)
        {
            case ChunkTypes::LeftTopCorner:
                return pos.x == 0 && pos.y + 1 == gridSize;
            case ChunkTypes::LeftBottomCorner:
                return pos.x == 0 && pos.y == 0;
            case ChunkTypes::RightTopCorner:
                return pos.x + 1 == gridSize && pos.y + 1 == gridSize;
            case ChunkTypes::RightBottomCorner:
                return pos.x + 1 == gridSize && pos.y == 0;
            default:
                break;
        }
        return false;
    };

    /* Returns the current number of vertices in the geometry vertex buffer */
    auto VertexCount = [&geometry]() -> unsigned int
    {
        return static_cast<unsigned int>(geometry.vertices.size());
    };

    auto CreateChunkBase = [&](Chunk& chunk)
    {
        for (GridPoint pos; pos.y < gridSize; ++pos.y)
        {
            for (pos.x = 0; pos.x < gridSize; ++pos.x)
                CreateQuad(pos.x*2, pos.y*2);
        }

        chunk.numVertices = VertexCount();
    };

    auto CreateChunkEdge = [&](Chunk& chunk, const ChunkTypes edgeType)
    {
        chunk.firstVertex = VertexCount();

        for (GridPoint pos; pos.y < gridSize; ++pos.y)
        {
            for (pos.x = 0; pos.x < gridSize; ++pos.x)
            {
                if (IsPosAtEdge(pos, edgeType))
                    CreateEdge(pos.x*2, pos.y*2, edgeType);
                else
                    CreateQuad(pos.x*2, pos.y*2);
            }
        }

        chunk.numVertices = VertexCount() - chunk.firstVertex;
    };

    auto CreateChunkCorner = [&](Chunk& chunk, const ChunkTypes cornerType)
    {
        chunk.firstVertex = VertexCount();

        for (GridPoint pos; pos.y < gridSize; ++pos.y)
        {
            for (pos.x = 0; pos.x < gridSize; ++pos.x)
            {
                if (IsPosAtCorner(pos, cornerType))
                    CreateCorner(pos.x*2, pos.y*2, cornerType);
                else if (IsPosAtEdge(pos, CornerTypeToEdgeType(pos, cornerType)))
                    CreateEdge(pos.x*2, pos.y*2, CornerTypeToEdgeType(pos, cornerType));
                else
                    CreateQuad(pos.x*2, pos.y*2);
            }
        }

        chunk.numVertices = VertexCount() - chunk.firstVertex;
    };

    /* --- /Internal functions --- */

    /* Initialize geometry settings */
    geometry.vertices.clear();
    geometry.indices.clear();

    geometry.primitiveType = Video::GeometryPrimitives::Triangles;

    /* Construct terrain geometry template */
    CreateChunkBase( geometry.GetGridChunk( ChunkTypes::Base ) );

    CreateChunkEdge( geometry.GetGridChunk( ChunkTypes::LeftEdge   ), ChunkTypes::LeftEdge   );
    CreateChunkEdge( geometry.GetGridChunk( ChunkTypes::RightEdge  ), ChunkTypes::RightEdge  );
    CreateChunkEdge( geometry.GetGridChunk( ChunkTypes::TopEdge    ), ChunkTypes::TopEdge    );
    CreateChunkEdge( geometry.GetGridChunk( ChunkTypes::BottomEdge ), ChunkTypes::BottomEdge );

    CreateChunkCorner( geometry.GetGridChunk( ChunkTypes::LeftTopCorner     ), ChunkTypes::LeftTopCorner     );
    CreateChunkCorner( geometry.GetGridChunk( ChunkTypes::LeftBottomCorner  ), ChunkTypes::LeftBottomCorner  );
    CreateChunkCorner( geometry.GetGridChunk( ChunkTypes::RightTopCorner    ), ChunkTypes::RightTopCorner    );
    CreateChunkCorner( geometry.GetGridChunk( ChunkTypes::RightBottomCorner ), ChunkTypes::RightBottomCorner );
}


} // /namespace GeometryGenerator

} // /namespace Scene

} // /namespace Fork



// ========================