/*
 * Mesh geometry header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MESH_GEOMETRY_H__
#define __FORK_MESH_GEOMETRY_H__


#include "Scene/Geometry/Node/Geometry.h"
#include "Video/RenderSystem/HardwareBuffer/VertexBuffer.h"
#include "Video/RenderSystem/HardwareBuffer/IndexBuffer.h"
#include "Video/RenderSystem/RenderState/GeometryPrimitives.h"


namespace Fork
{

namespace Scene
{


/**
Mesh geometry base class. When you want to create your own geometry class which is to be rendered,
use this as base class. See "Simple3DMeshGeometry" for instance how this can be done.
\see CommonMeshGeometry
\see Simple2DMeshGeometry
\see Simple3DMeshGeometry
\see TangentSapceMeshGeometry
\ingroup std_geometries
*/
class FORK_EXPORT MeshGeometry : public Geometry
{
    
    public:
        
        virtual ~MeshGeometry();

        MeshGeometry(const MeshGeometry&) = delete;
        MeshGeometry& operator = (const MeshGeometry&) = delete;

        /* === Functions === */

        virtual void Visit(SceneVisitor* visitor);

        //virtual Video::VertexFormat VertexFormat() const = 0;
        //virtual Video::IndexFormat IndexFormat() const = 0;

        /**
        Sets up the data for an un-indexed geometry. This will upload the passed data to the hardware vertex buffer.
        \param[in] vertexFormat Specifies the vertex format.
        \param[in] vertexData Raw pointer to the vertex data. This can be your custom vertex buffer. Here are a few examples:
        \code
        // Vertex buffer with 3D coordinates only (enough for a triangle or three single points):
        Math::Vector3f verticesA[] = { Math::Vector3f(0, 1, 0), Math::Vector3f(1, -1, 0), Math::Vector3f(-1, -1, 0) };
        
        // Simple vertex buffer (with std::vector, never use std::list, the data must be contiguous):
        std::vector<Video::Simple2DVertex> verticesB;
        
        // Tangent space vertex buffer:
        std::vector<Video::TangentSpaceVertex> verticesC;
        \endcode
        \param[in] vertexDataSize Specifies the size (in bytes) of the entire vertex data buffer.
        \see Video::GeometryPrimitives
        \see Video::VertexFormat
        \throws NullPointerException If 'vertexData' is null.
        \throws InvalidArgumentException If 'vertexDataSize' is zero.
        \throws RenderContextException If no render context has been created.
        */
        void SetupHardwareBuffer(
            const Video::VertexFormat& vertexFormat, const void* vertexData, size_t vertexDataSize
        );

        /**
        Sets up the data for an indexed geometry. This will upload the passed data to the hardware vertex- and index buffers.
        \param[in] vertexFormat Specifies the vertex format.
        \param[in] vertexData Raw pointer to the vertex data. This can be your custom vertex buffer. Here are a few examples:
        \code
        // Vertex buffer with 3D coordinates only (enough for a triangle or three single points):
        Math::Vector3f verticesA[] = { Math::Vector3f(0, 1, 0), Math::Vector3f(1, -1, 0), Math::Vector3f(-1, -1, 0) };
        
        // Simple vertex buffer (with std::vector, never use std::list, the data must be contiguous):
        std::vector<Video::Simple2DVertex> verticesB;
        
        // Tangent space vertex buffer:
        std::vector<Video::TangentSpaceVertex> verticesC;
        \endcode
        \param[in] vertexDataSize Specifies the size (in bytes) of the entire vertex data buffer.
        \param[in] indexFormat Specifies the index format.
        \param[in] indexData Raw pointer to the index data. This should be an array container
        for unsigned- byte, short or integer. Here are a few examples:
        \code
        std::vector<unsigned char> ubyteIndices;
        std::vector<unsigned short> ushortIndices;
        std::vector<unsigned int> uintIndices;
        \endcode
        \param[in] indexDataSize Specifies the size (in bytes) of the entire index data buffer.
        \throws NullPointerException If 'vertexData' or 'indexData' is null.
        \throws InvalidArgumentException If 'vertexDataSize' or 'indexDataSize' is zero.
        \throws RenderContextException If no render context has been created.
        \see GetVertexBuffer
        \see GetIndexBuffer
        \see primitiveType
        */
        void SetupHardwareBuffer(
            const Video::VertexFormat& vertexFormat, const void* vertexData, size_t vertexDataSize,
            const Video::IndexFormat& indexFormat, const void* indexData, size_t indexDataSize
        );

        /**
        Updates the hardware vertex buffer. The vertex format of the passed data must match the vertex format
        of the hardware vertex buffer when it was created with the "SetupHardwareBuffer" function.
        \param[in] vertexData Raw pointer to the vertex data. This can be your custom vertex buffer.
        \param[in] vertexDataSize Specifies the size (in bytes) of the entire vertex data buffer.
        \param[in] offset Optional offset (in bytes) where to start with copying within the hardware buffer.
        \throws InvalidStateException If the vertex buffer has not already been created (use "SetupHardwareBuffer" first).
        \note Make sure there is an active render system and render context!
        \see SetupHardwareBuffer
        */
        void UpdateVertexBuffer(const void* vertexData, size_t vertexDataSize, size_t offset = 0);
        /**
        Updates the hardware index buffer. The index format of the passed data must match the index format
        of the hardware index buffer when it was created with the "SetupHardwareBuffer" function.
        \param[in] indexData Raw pointer to the index data. This can be your custom index buffer.
        \param[in] indexDataSize Specifies the size (in bytes) of the entire index data buffer.
        \param[in] offset Optional offset (in bytes) where to start with copying within the hardware buffer.
        \throws InvalidStateException If the index buffer has not already been created (use "SetupHardwareBuffer" first).
        \note Make sure there is an active render system and render context!
        \see SetupHardwareBuffer
        */
        void UpdateIndexBuffer(const void* indexData, size_t indexDataSize, size_t offset = 0);

        //! Returns the number of vertices or zero if the vertices are only stored on the GPU.
        virtual size_t NumVertices() const;
        //! Returns the number of indices or zero if the indices are only stored on the GPU.
        virtual size_t NumIndices() const;

        //! Returns a raw-pointer to the vertex buffer.
        inline Video::VertexBuffer* GetVertexBuffer() const
        {
            return vertexBuffer.get();
        }
        //! Returns a raw-pointer to the index buffer.
        inline Video::IndexBuffer* GetIndexBuffer() const
        {
            return indexBuffer.get();
        }

        /* === Members === */

        //! Primitive geometry type. By default Video::GeometryPrimitives::Triangles.
        Video::GeometryPrimitives primitiveType = Video::GeometryPrimitives::Triangles;

    protected:
        
        MeshGeometry() = default;

        /* === Members === */

        Video::VertexBufferPtr  vertexBuffer;   //!< Hardware vertex buffer.
        Video::IndexBufferPtr   indexBuffer;    //!< Hardware index buffer.

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================