/*
 * Mesh geometry atlas header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MESH_GEOMETRY_ATLAS_H__
#define __FORK_MESH_GEOMETRY_ATLAS_H__


#include "Scene/Geometry/Node/MeshGeometry.h"

#include <vector>


namespace Fork
{

namespace Scene
{


/**
Mesh geometry atlas class. This class holds a list of
mesh partitions (with first vertex index and number of vertices).
*/
class FORK_EXPORT MeshGeometryAtlas
{
    
    public:
        
        //! Atlas partition structure.
        struct Partition
        {
            unsigned int numVertices = 0;
            unsigned int firstVertex = 0;
        };

        MeshGeometryAtlas(const MeshGeometry& geometry);

        /**
        Records a new mesh partition. This depends on the current
        number of vertices and indices and the previous partition.
        \return The new recorded partition.
        \see Partition
        */
        Partition RecordPartition();

        //! Returns the list of all atlases.
        inline const std::vector<Partition>& GetPartitions() const
        {
            return partitions_;
        }

    private:
        
        const MeshGeometry* geometry_;
        std::vector<Partition> partitions_;

        size_t vertexCounter_ = 0;
        size_t indexCounter_ = 0;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================