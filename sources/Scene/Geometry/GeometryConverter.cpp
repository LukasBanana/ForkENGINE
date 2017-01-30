/*
 * Geometry converter file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/GeometryConverter.h"
#include "Scene/Geometry/Node/CompositionGeometry.h"
#include "Core/Exception/IndexOutOfBoundsException.h"
#include "Core/Exception/InvalidArgumentException.h"
#include "Math/Common/CoordinateSpace.h"
#include "Math/Geometry/Triangle.h"

#include "Optimizer/GeometryTextureOptimizer.h"
#include "Optimizer/GeometryCleanUpOptimizer.h"

#include <algorithm>


namespace Fork
{

namespace Scene
{

namespace GeometryConverter
{


/*
 * Internal structures
 */

/**
Structure to store a vector and a reference counter,
to normalize it correctly when several triangles share their vertices.
*/
struct VectorRef
{
    void Add(const Math::Vector3f& vec)
    {
        vector += vec;
        ++counter;
    }
    Math::Vector3f& Normalize()
    {
        if (counter > 1)
            vector /= static_cast<float>(counter);
        return vector;
    }

    Math::Vector3f  vector;
    int             counter = 0;
};


/*
 * Global functions
 */

static unsigned int FetchIndexFromBuffer(
    const CommonIndexBuffer& indexBuffer, size_t numVertices, const char* functionName, size_t index)
{
    auto vertIndex = indexBuffer[index];
    if (vertIndex >= numVertices)
        throw IndexOutOfBoundsException(functionName, vertIndex);
    return vertIndex;
};

FORK_EXPORT void GenerateNormalsFlat(
    Video::AttributeConstIterator coordIterator,
    Video::AttributeIterator normalIterator,
    const CommonIndexBuffer& indexBuffer)
{
    /* Get number of vertices */
    if (coordIterator.GetCount() != normalIterator.GetCount())
    {
        throw InvalidArgumentException(
            __FUNCTION__, "coordIterator/normalIterator",
            "Number of vertex coordinates and normals do not match"
        );
    }

    const auto numVertices = coordIterator.GetCount();
    const auto functionName = __FUNCTION__;

    auto FetchIndex = [&indexBuffer, &numVertices, &functionName](size_t index) -> unsigned int
    {
        return FetchIndexFromBuffer(indexBuffer, numVertices, functionName, index);
    };

    /* Store normal vectors in separate list to count how many triangles use the same vertex (ref-counter) */
    std::vector<VectorRef> normalBuffer(numVertices);

    /* Iterate over all indices in the <triangle list> */
    for (size_t i = 0, n = indexBuffer.size(); i < n; i += 3)
    {
        /* Fetch indices */
        auto v0 = FetchIndex(i    );
        auto v1 = FetchIndex(i + 1);
        auto v2 = FetchIndex(i + 2);

        /* Fetch vertex coordinates */
        const auto& coord0 = coordIterator.Get<Math::Vector3f>(v0);
        const auto& coord1 = coordIterator.Get<Math::Vector3f>(v1);
        const auto& coord2 = coordIterator.Get<Math::Vector3f>(v2);

        /* Compute normal vector */
        auto normal = Math::Cross(coord1 - coord0, coord2 - coord0);
        Normalize(normal);

        /* Add normal to separate buffer and count references */
        normalBuffer[v0].Add(normal);
        normalBuffer[v1].Add(normal);
        normalBuffer[v2].Add(normal);
    }

    /* Finally normalize all vertices and store it in the output normal buffer */
    for (size_t i = 0; i < numVertices; ++i)
        normalIterator.Get<Math::Vector3f>(i) = normalBuffer[i].Normalize();
}

FORK_EXPORT void GenerateNormalsSmooth(
    Video::AttributeConstIterator coordIterator,
    Video::AttributeIterator normalIterator,
    const CommonIndexBuffer& indexBuffer,
    float angleThreshold)
{
    //todo...
}

FORK_EXPORT void GenerateTangentSpaceFlat(
    Video::AttributeConstIterator coordIterator,
    Video::AttributeConstIterator texCoordIterator,
    Video::AttributeIterator tangentIterator,
    Video::AttributeIterator bitangentIterator,
    Video::AttributeIterator normalIterator,
    const CommonIndexBuffer& indexBuffer,
    bool ignoreNormals)
{
    /* Get number of vertices */
    if ( coordIterator.GetCount() != texCoordIterator .GetCount() ||
         coordIterator.GetCount() != tangentIterator  .GetCount() ||
         coordIterator.GetCount() != bitangentIterator.GetCount() ||
         coordIterator.GetCount() != normalIterator   .GetCount() )
    {
        throw InvalidArgumentException(
            __FUNCTION__, "<Some Attribute Iterators>",
            "Number of vertices in attribute iterators do not match"
        );
    }

    const auto numVertices = coordIterator.GetCount();
    const auto functionName = __FUNCTION__;

    auto FetchIndex = [&indexBuffer, &numVertices, &functionName](size_t index) -> unsigned int
    {
        return FetchIndexFromBuffer(indexBuffer, numVertices, functionName, index);
    };
    
    /* Store normal vectors in separate list to count how many triangles use the same vertex (ref-counter) */
    std::vector<VectorRef> tangentBuffer(numVertices);
    std::vector<VectorRef> bitangentBuffer(numVertices);
    std::vector<VectorRef> normalBuffer(numVertices);

    Math::Triangle3f triangleCoords;
    Math::Triangle2f triangleTexCoords;

    /* Iterate over all indices in the <triangle list> */
    for (size_t i = 0, n = indexBuffer.size(); i < n; i += 3)
    {
        /* Fetch indices */
        auto v0 = FetchIndex(i    );
        auto v1 = FetchIndex(i + 1);
        auto v2 = FetchIndex(i + 2);

        /* Fetch vertex- and texture coordinates */
        triangleCoords.a    = coordIterator     .Get<Math::Vector3f>(v0);
        triangleTexCoords.a = texCoordIterator  .Get<Math::Vector2f>(v0);

        triangleCoords.b    = coordIterator     .Get<Math::Vector3f>(v1);
        triangleTexCoords.b = texCoordIterator  .Get<Math::Vector2f>(v1);

        triangleCoords.c    = coordIterator     .Get<Math::Vector3f>(v2);
        triangleTexCoords.c = texCoordIterator  .Get<Math::Vector2f>(v2);

        /* Compute tangent space */
        auto tangentSpace = Math::ComputeTangentSpace(triangleCoords, triangleTexCoords);

        /* Always add tangent and bitangent to separate buffers and count references */
        tangentBuffer[v0].Add(tangentSpace.GetColumn(0));
        tangentBuffer[v1].Add(tangentSpace.GetColumn(0));
        tangentBuffer[v2].Add(tangentSpace.GetColumn(0));

        bitangentBuffer[v0].Add(tangentSpace.GetColumn(1));
        bitangentBuffer[v1].Add(tangentSpace.GetColumn(1));
        bitangentBuffer[v2].Add(tangentSpace.GetColumn(1));

        /* Add normal to separate buffer and count references if enabled */
        if (!ignoreNormals)
        {
            normalBuffer[v0].Add(tangentSpace.GetColumn(2));
            normalBuffer[v1].Add(tangentSpace.GetColumn(2));
            normalBuffer[v2].Add(tangentSpace.GetColumn(2));
        }
    }

    /* Finally normalize all vertices and store it in the output normal buffer */
    for (size_t i = 0; i < numVertices; ++i)
    {
        /* Always store tangent and bitangent */
        tangentIterator  .Get<Math::Vector3f>(i) = tangentBuffer  [i].Normalize();
        bitangentIterator.Get<Math::Vector3f>(i) = bitangentBuffer[i].Normalize();

        /* Store normal if enabled */
        if (!ignoreNormals)
            normalIterator.Get<Math::Vector3f>(i) = normalBuffer[i].Normalize();
    }
}

FORK_EXPORT void GenerateTangentSpaceSmooth(
    Video::AttributeConstIterator coordIterator,
    Video::AttributeConstIterator texCoordIterator,
    Video::AttributeIterator tangentIterator,
    Video::AttributeIterator bitangentIterator,
    Video::AttributeIterator normalIterator,
    const CommonIndexBuffer& indexBuffer,
    float angleThreshold,
    bool ignoreNormals)
{
    //todo ...
}

FORK_EXPORT void OptimizeGeometryGraph(GeometryPtr& geometry)
{
    if (geometry)
    {
        GeometryTextureOptimizer().OptimizeGeometryGraph(geometry);
        GeometryCleanUpOptimizer().OptimizeGeometryGraph(geometry);
    }
}

FORK_EXPORT void ComputeBoundingVolume(BoundingVolume& boundingVolume, Video::AttributeConstIterator coordIterator)
{
    /* Compute bounding box */
    boundingVolume.box.Invalidate();

    for (; coordIterator.HasNext(); coordIterator.Next())
        boundingVolume.box.InsertPoint(coordIterator.Get<Math::Point3f>());

    /* Compute bounding sphere */
    boundingVolume.sphere.point = boundingVolume.box.Center();
    boundingVolume.sphere.radius = 0.0f;
    
    for (coordIterator.Reset(); coordIterator.HasNext(); coordIterator.Next())
    {
        /* Store maximal (squared) distance -> normalize at the end */
        boundingVolume.sphere.radius = std::max(
            boundingVolume.sphere.radius,
            Math::DistanceSq(coordIterator.Get<Math::Point3f>(), boundingVolume.sphere.point)
        );
    }

    boundingVolume.sphere.radius = std::sqrt(boundingVolume.sphere.radius);

    /* Determine bounding volume type */
    boundingVolume.DetermineType();
}

FORK_EXPORT void ComputeBoundingVolume(BoundingVolume& boundingVolume, const std::vector<GeometryPtr>& geometries)
{
    /* Compute bounding box */
    boundingVolume.box.Invalidate();

    for (auto& geometry : geometries)
    {
        geometry->ComputeBoundingVolume();
        boundingVolume.box.InsertBox(geometry->boundingVolume.box);
    }

    /* Compute bounding sphere */
    boundingVolume.sphere.radius = 0.0f;
    boundingVolume.sphere.point = boundingVolume.box.Center();

    for (const auto& geometry : geometries)
    {
        const auto& sphere = geometry->boundingVolume.sphere;

        /*
        Store maximal [ distance (between main sphere and current sphere) plus current sphere radius ].
        ( No optimization with separated radius normalization possible here,
          because the sub-geometry radius is already noramlized.
          Normalized and unnormalized radii can not be mixed! )
        */
        boundingVolume.sphere.radius = std::max(
            boundingVolume.sphere.radius,
            Math::Distance(boundingVolume.sphere.point, sphere.point) + sphere.radius
        );
    }

    /* Determine bounding volume type */
    boundingVolume.DetermineType();
}


} // /namespace GeometryConverter

} // /namespace Scene

} // /namespace Fork



// ========================