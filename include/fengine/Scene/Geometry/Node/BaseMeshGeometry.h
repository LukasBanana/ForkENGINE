/*
 * Base mesh geometry header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_BASE_MESH_GEOMETRY_H__
#define __FORK_BASE_MESH_GEOMETRY_H__


#include "Scene/Geometry/Node/MeshGeometry.h"
#include "Scene/Geometry/GeometryConverter.h"
#include "Core/Exception/IndexOutOfBoundsException.h"

#include <initializer_list>


namespace Fork
{

namespace Scene
{


/**
Base mesh geometry class. This is the used the classes:
CommonMeshGeometry, Simple2DMeshGeometry, Simple3DMeshGeometry, TangentSpaceMeshGeometry.
\tparam VtxT Specifies the vertex structure type.
This type must implement the following static function:
\code
// Returns the vertex format.
static Video::VertexFormat Format()
\endcode
This could be: Video::CommonVertex, Video::Simple2DVertex, Video::Simple3DVertex, Video::TangentSpaceVertex.
\tparam IdxT Specifies the index type. This should be: 'unsigned char', 'unsigned short' or 'unsigned int'.
\see Video::VertexFormat
\see CommonIndexBuffer
*/
template <class VtxT, typename IdxT> class BaseMeshGeometry : public MeshGeometry
{
    
    public:
        
        BaseMeshGeometry() = default;

        BaseMeshGeometry(const BaseMeshGeometry<VtxT, IdxT>&) = delete;
        BaseMeshGeometry<VtxT, IdxT>& operator = (const BaseMeshGeometry<VtxT, IdxT>&) = delete;

        virtual ~BaseMeshGeometry()
        {
        }

        /* === Functions === */

        //! Computes the bounding volume of this mesh geometry.
        void ComputeBoundingVolume() override
        {
            GeometryConverter::ComputeBoundingVolume(
                boundingVolume,
                Video::AttributeConstIterator(&vertices[0].coord, vertices.size(), sizeof(VtxT))
            );
        }

        /**
        Uploads the geometry data to the GPU. The index buffer is optional.
        \remarks If the hardware buffer should only be updated (and not re-created),
        then use "UpdateVertexBuffer" and/or "UpdateIndexBuffer".
        If the vertex- or index format has changed (for whatever reason)
        or the hardware buffer has not already been created, this function must be called!
        \see MeshGeometry::SetupHardwareBuffer
        \see UpdateVertexBuffer
        \see UpdateIndexBuffer
        */
        void SetupHardwareBuffer()
        {
            if (indices.empty())
            {
                MeshGeometry::SetupHardwareBuffer(
                    VtxT::Format(),
                    vertices.data(), sizeof(VtxT)*vertices.size()
                );
            }
            else
            {
                MeshGeometry::SetupHardwareBuffer(
                    // vertices
                    VtxT::Format(),
                    vertices.data(), sizeof(VtxT)*vertices.size(),
                    // indices
                    Video::IndexFormat(Video::GetDataType<IdxT>()),
                    indices.data(), sizeof(IdxT)*indices.size()
                );
            }
        }

        /**
        Updates the entire vertex buffer.
        \see MeshGeometry::UpdateVertexBuffer
        */
        void UpdateVertexBuffer()
        {
            MeshGeometry::UpdateVertexBuffer(vertices.data(), sizeof(VtxT)*vertices.size());
        }
        /**
        Updates the vertex buffer partially.
        \throws IndexOutOfBoundsException If (firstVertex + numVertices) is greater than the number of vertices in the vertex buffer.
        \see MeshGeometry::UpdateVertexBuffer
        */
        void UpdateVertexBuffer(size_t firstVertex, size_t numVertices)
        {
            if (firstVertex + numVertices > vertices.size())
                throw IndexOutOfBoundsException(__FUNCTION__, firstVertex + numVertices);
            MeshGeometry::UpdateVertexBuffer(vertices.data() + firstVertex, sizeof(VtxT)*numVertices, sizeof(VtxT)*firstVertex);
        }

        /**
        Updates the entire index buffer.
        \see MeshGeometry::UpdateVertexBuffer
        */
        void UpdateIndexBuffer()
        {
            MeshGeometry::UpdateIndexBuffer(indices.data(), sizeof(VtxT)*indices.size());
        }
        /**
        Updates the index buffer partially.
        \throws IndexOutOfBoundsException If (firstIndex + numIndices) is greater than the number of indices in the index buffer.
        \see MeshGeometry::UpdateIndexBuffer
        */
        void UpdateIndexBuffer(size_t firstIndex, size_t numIndices)
        {
            if (firstIndex + numIndices > vertices.size())
                throw IndexOutOfBoundsException(__FUNCTION__, firstIndex + numIndices);
            MeshGeometry::UpdateIndexBuffer(vertices.data() + firstIndex, sizeof(VtxT)*numIndices, sizeof(VtxT)*firstIndex);
        }

        /**
        Ensures that the hardware buffer is created.
        This will only upload the geometry data to the GPU if the vertex- and/or index buffer has not already been created.
        To force an upload of the geometry data to the GPU use "SetupHardwareBuffer".
        \see SetupHardwareBuffer
        */
        void EnsureHardwareBuffer()
        {
            if (!vertexBuffer || (!indices.empty() && !indexBuffer))
                SetupHardwareBuffer();
        }

        //! Adds a new vertex. This can also be done by adding new elements to the "vertices" data member.
        void AddVertex(const VtxT& vertex)
        {
            vertices.push_back(vertex);
        }

        /**
        Adds the specified vertices. This can also be done by adding new elements to the "vertices" data member.
        \param[in] newVertices Specifies a list of vertices which will be added to the vertices list.
        */
        void AddVertices(const std::initializer_list<VtxT>& newVertices)
        {
            vertices.insert(vertices.end(), newVertices.begin(), newVertices.end());
        }

        //! Adds a new index. This can also be done by adding new elements to the "indices" data member.
        void AddIndex(const IdxT& index)
        {
            indices.push_back(index);
        }

        /**
        Adds the specified indices. This can also be done by adding new elements to the "indices" data member.
        \param[in] newIndices Specifies a list of indices which will be added to the indices list.
        \param[in] indexOffset Specifies an offset which will be added to each index from the intializer list if its not zero.
        */
        void AddIndices(const std::initializer_list<IdxT>& newIndices, const IdxT& indexOffset = 0)
        {
            if (indexOffset > 0)
            {
                for (auto i : newIndices)
                    indices.push_back(i + indexOffset);
            }
            else
                indices.insert(indices.end(), newIndices.begin(), newIndices.end());
        }

        /**
        Validates the indices. If any index is greater then or equal to the number of vertices, an exception is thrown.
        \throws IndexOutOfBoundsException If a vertex index is out of bounds.
        */
        void Validate()
        {
            const auto numVerts = vertices.size();
            for (auto idx : indices)
            {
                if (idx >= numVerts)
                    throw IndexOutOfBoundsException(__FUNCTION__, size_t(idx));
            }
        }

        //! Clears the vertices and indices.
        void Clear()
        {
            vertices.clear();
            indices.clear();
        }

        size_t NumVertices() const
        {
            return vertices.size();
        }

        size_t NumIndices() const
        {
            return indices.size();
        }

        /* === Members === */

        std::vector<VtxT> vertices; //!< Vertex list.
        std::vector<IdxT> indices;  //!< Index list.

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================