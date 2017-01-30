/*
 * Mesh geometry atlas file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/MeshGeometryAtlas.h"


namespace Fork
{

namespace Scene
{


MeshGeometryAtlas::MeshGeometryAtlas(const MeshGeometry& geometry) :
    geometry_{ &geometry }
{
}

MeshGeometryAtlas::Partition MeshGeometryAtlas::RecordPartition()
{
    /* Get previous partition */
    Partition prevPartition;

    if (!partitions_.empty())
        prevPartition = partitions_.back();

    /* Setup new partition */
    Partition partition;

    if (geometry_->NumIndices() > 0)
    {
        partition.numVertices = geometry_->NumIndices() - indexCounter_;
        partition.firstVertex = vertexCounter_;
    }
    else
    {
        partition.numVertices = geometry_->NumVertices() - vertexCounter_;
        partition.firstVertex = vertexCounter_;
    }

    /* Store new number of vertices and indices */
    vertexCounter_ = geometry_->NumVertices();
    indexCounter_ = geometry_->NumIndices();

    /* Add and return new partition */
    partitions_.push_back(partition);

    return partition;
}


} // /namespace Scene

} // /namespace Fork



// ========================