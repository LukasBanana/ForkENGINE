/*
 * Terrain geometry file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Node/TerrainGeometry.h"


namespace Fork
{

namespace Scene
{


TerrainGeometry::~TerrainGeometry()
{
}

void TerrainGeometry::Visit(SceneVisitor* visitor)
{
    visitor->VisitTerrainGeometry(this);
}

Geometry::Types TerrainGeometry::Type() const
{
    return Geometry::Types::Terrain;
}

//!TODO! -> the bounding volume's transformation works differently for terrains!!!
void TerrainGeometry::ComputeBoundingVolume()
{
    if (templateGeometry)
    {
        templateGeometry->ComputeBoundingVolume();
        boundingVolume = templateGeometry->boundingVolume;
    }
}


} // /namespace Scene

} // /namespace Fork



// ========================