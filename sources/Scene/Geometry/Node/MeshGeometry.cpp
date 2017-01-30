/*
 * Mesh geometry file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Node/MeshGeometry.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/Exception/InvalidArgumentException.h"
#include "Core/Exception/InvalidStateException.h"
#include "Video/RenderSystem/RenderContextException.h"
#include "Video/RenderSystem/RenderContext.h"
#include "Video/RenderSystem/RenderSystem.h"


namespace Fork
{

namespace Scene
{


MeshGeometry::~MeshGeometry()
{
}

void MeshGeometry::SetupHardwareBuffer(
    const Video::VertexFormat& vertexFormat, const void* vertexData, size_t vertexDataSize)
{
    /* Check for valid arguments */
    ASSERT_POINTER(vertexData);

    if (!vertexDataSize)
        throw InvalidArgumentException(__FUNCTION__, "vertexDataSize", "Must be greater than zero");

    /* Get active render system */
    auto renderSystem = Video::RenderSystem::Active();
    if (!renderSystem || !Video::RenderContext::Active())
        throw RenderContextException("Render context is required for \"" __FUNCTION__ "\" but has not been created");

    /* Write hardware buffer */
    if (!vertexBuffer)
        vertexBuffer = renderSystem->CreateVertexBuffer();

    renderSystem->WriteBuffer(vertexBuffer.get(), vertexFormat, vertexData, vertexDataSize);

    /* Release previous index buffer */
    indexBuffer = nullptr;
}

void MeshGeometry::SetupHardwareBuffer(
    const Video::VertexFormat& vertexFormat, const void* vertexData, size_t vertexDataSize,
    const Video::IndexFormat& indexFormat, const void* indexData, size_t indexDataSize)
{
    /* Check for valid arguments */
    ASSERT_POINTER(vertexData);
    ASSERT_POINTER(indexData);

    if (!vertexDataSize)
        throw InvalidArgumentException(__FUNCTION__, "vertexDataSize", "Must be greater than zero");
    if (!indexDataSize)
        throw InvalidArgumentException(__FUNCTION__, "indexDataSize", "Must be greater than zero");

    /* Get active render system */
    auto renderSystem = Video::RenderSystem::Active();
    if (!renderSystem || !Video::RenderContext::Active())
        throw RenderContextException("Render context is required for \"" __FUNCTION__ "\" but has not been created");

    /* Write hardware buffer */
    if (!vertexBuffer)
        vertexBuffer = renderSystem->CreateVertexBuffer();

    renderSystem->WriteBuffer(vertexBuffer.get(), vertexFormat, vertexData, vertexDataSize);

    /* Write index buffer */
    if (!indexBuffer)
        indexBuffer = renderSystem->CreateIndexBuffer();

    renderSystem->WriteBuffer(indexBuffer.get(), indexFormat, indexData, indexDataSize);
}

void MeshGeometry::UpdateVertexBuffer(const void* vertexData, size_t vertexDataSize, size_t offset)
{
    if (vertexBuffer)
        Video::RenderSystem::Active()->WriteSubBuffer(vertexBuffer.get(), vertexData, vertexDataSize, offset);
    else
        throw InvalidStateException(__FUNCTION__, "Vertex buffer has not been created");
}

void MeshGeometry::UpdateIndexBuffer(const void* indexData, size_t indexDataSize, size_t offset)
{
    if (indexBuffer)
        Video::RenderSystem::Active()->WriteSubBuffer(indexBuffer.get(), indexData, indexDataSize, offset);
    else
        throw InvalidStateException(__FUNCTION__, "Index buffer has not been created");
}

void MeshGeometry::Visit(SceneVisitor* visitor)
{
    visitor->VisitMeshGeometry(this);
}

size_t MeshGeometry::NumVertices() const
{
    /* Return zero here, because this class stores the vertices only on the GPU */
    return 0;
}

size_t MeshGeometry::NumIndices() const
{
    /* Return zero here, because this class stores the indices only on the GPU */
    return 0;
}


} // /namespace Scene

} // /namespace Fork



// ========================