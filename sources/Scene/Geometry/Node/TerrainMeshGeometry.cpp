/*
 * Terrain mesh geometry file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Node/TerrainMeshGeometry.h"
#include "Core/Exception/IndexOutOfBoundsException.h"
#include "Core/StaticConfig.h"


namespace Fork
{

namespace Scene
{


void TerrainMeshGeometry::Visit(SceneVisitor* visitor)
{
    visitor->VisitTerrainMeshGeometry(this);
}

Geometry::Types TerrainMeshGeometry::Type() const
{
    return Geometry::Types::TerrainMesh;
}

void TerrainMeshGeometry::AddVertex(const Math::Point2f& coord)
{
    vertices.push_back({ coord });
}

TerrainMeshGeometry::GridChunk& TerrainMeshGeometry::GetGridChunk(const GridChunkTypes type)
{
    auto index = static_cast<size_t>(type);
    #ifdef FORK_DEBUG
    if (index >= gridChunks.size())
        throw IndexOutOfBoundsException(__FUNCTION__, index);
    #endif
    return gridChunks[index];
}

const TerrainMeshGeometry::GridChunk& TerrainMeshGeometry::GetGridChunk(const GridChunkTypes type) const
{
    auto index = static_cast<size_t>(type);
    #ifdef FORK_DEBUG
    if (index >= gridChunks.size())
        throw IndexOutOfBoundsException(__FUNCTION__, index);
    #endif
    return gridChunks[index];
}


} // /namespace Scene

} // /namespace Fork



// ========================