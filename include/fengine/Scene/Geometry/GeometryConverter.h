/*
 * Geometry converter header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GEOMETRY_CONVERTER_H__
#define __FORK_GEOMETRY_CONVERTER_H__


#include "Core/Export.h"
#include "Math/Geometry/AABB.h"
#include "Math/Geometry/Sphere.h"
#include "Math/Common/Convert.h"
#include "Video/BufferFormat/AttributeIterator.h"
#include "Scene/Geometry/Node/Geometry.h"
#include "Scene/Geometry/BoundingVolume.h"

#include <algorithm>
#include <vector>
#include <cmath>


namespace Fork
{

//! Common type for index buffers.
typedef std::vector<unsigned int> CommonIndexBuffer;

namespace Scene
{

class Geometry;

/**
Geometry converter namespace. As the name implies, it provides functions to convert geometry data.
But there are several functions which are used to not convert geometry but to extract meta data, such as bounding boxes.
*/
namespace GeometryConverter
{


/* --- Global functions --- */

/**
Generates flat normals for the specified vertex range.
\param[in] coordIterator Specifies the attribute iterator for the vertex coordinates.
\param[in,out] normalIterator Specifies the attribute iterator for the normal vectors.
\param[in] indexBuffer Specifies the index buffer. This will be treated
as indices of a triangle list. Therefore it must be a multiple of three!
Otherwise the last one or two indices will be ignored.
\remarks Usage example:
\code
std::vector<Video::Simple3DVertex> vertices;
Scene::GeometryConverter::GenerateNormalsFlat(
    Video::AttributeConstIterator(&vertices[0].coord, vertices.size(), sizeof(Video::TangentSpaceVertex)),
    Video::AttributeIterator(&vertices[0].normal, vertices.size(), sizeof(Video::Simple3DVertex)),
    indexBuffer
);
\endcode
\note 'coordIterator' must be an attribute iterator which points to the vertex cooordinates and
these coordinates must consist of three 32-bit floats!
'normalIterator' must be an attribute iterator which points to the vertex normals and
these normals must consist of three 32-bit floats!
\throws IndexOutOfBoundsException If any index of 'indexBuffer' is out of bounds.
\throws InvalidArgumentException If the number of vertex coordinates and normals don't match.
\see GenerateNormalsSmooth
*/
FORK_EXPORT void GenerateNormalsFlat(
    Video::AttributeConstIterator coordIterator,
    Video::AttributeIterator normalIterator,
    const CommonIndexBuffer& indexBuffer
);
/**
Generates smooth normals for the specified vertex range.
\see GenerateNormalsFlat
\todo Not yet implemented!
*/
FORK_EXPORT void GenerateNormalsSmooth(
    Video::AttributeConstIterator coordIterator,
    Video::AttributeIterator normalIterator,
    const CommonIndexBuffer& indexBuffer,
    float angleThreshold = -1.0f
);

/**
Generates flat tangent space vectors for the specified vertex range.
\param[in] coordIterator Specifies the attribute iterator for the vertex coordinates.
\param[in] texCoordIterator Specifies the attribute iterator for the texture coordinates.
\param[in,out] tangentIterator Specifies the attribute iterator for the tangent vectors.
\param[in,out] bitangentIterator Specifies the attribute iterator for the bitangent (also called "binormal") vectors.
\param[in,out] normalIterator Specifies the attribute iterator for the normal vectors.
\param[in] indexBuffer Specifies the index buffer. This will be treated
as indices of a triangle list. Therefore it must be a multiple of three!
Otherwise the last one or two indices will be ignored.
\param[in] ignoreNormals Specifies whether the normals are to be ignored or not.
This can be used to keep the normals as they are when the mesh was loaded from file. By default false.
\remarks Usage example:
\code
std::vector<Video::TangentSpaceVertex> vertices;
Scene::GeometryConverter::GenerateTangentSpaceFlat(
    Video::AttributeConstIterator(&vertices[0].coord, vertices.size(), sizeof(Video::TangentSpaceVertex)),
    Video::AttributeConstIterator(&vertices[0].texCoord, vertices.size(), sizeof(Video::TangentSpaceVertex)),
    Video::AttributeIterator(&vertices[0].normal, vertices.size(), sizeof(Video::TangentSpaceVertex)),
    Video::AttributeIterator(&vertices[0].tangent, vertices.size(), sizeof(Video::TangentSpaceVertex)),
    Video::AttributeIterator(&vertices[0].bitangent, vertices.size(), sizeof(Video::TangentSpaceVertex)),
    indexBuffer
);
\endcode
\note 'texCoordIterator' must be an attribute iterator whose elements consist of at least two 32-bit floats.
All other attribute iterators must consist of at least three 32-bit floats.
\throws IndexOutOfBoundsException If any index of 'indexBuffer' is out of bounds.
\throws InvalidArgumentException If the number of elements in all attribute iterators don't match.
\see GenerateTangentSpaceSmooth
*/
FORK_EXPORT void GenerateTangentSpaceFlat(
    Video::AttributeConstIterator coordIterator,
    Video::AttributeConstIterator texCoordIterator,
    Video::AttributeIterator tangentIterator,
    Video::AttributeIterator bitangentIterator,
    Video::AttributeIterator normalIterator,
    const CommonIndexBuffer& indexBuffer,
    bool ignoreNormals = false
);
/**
Generates smooth tangent space vectors for the specified vertex range.
\see GenerateTangentSpaceFlat
\todo Not yet implemented!
*/
FORK_EXPORT void GenerateTangentSpaceSmooth(
    Video::AttributeConstIterator coordIterator,
    Video::AttributeConstIterator texCoordIterator,
    Video::AttributeIterator tangentIterator,
    Video::AttributeIterator bitangentIterator,
    Video::AttributeIterator normalIterator,
    const CommonIndexBuffer& indexBuffer,
    float angleThreshold = -1.0f,
    bool ignoreNormals = false
);

/**
Optimizes the specified geometry graph. This will optimize especially the texture usages,
i.e. it will group geometry to a composition which uses the same texture constellation.
This is done in several optimization passes like it's usual in optimizing compilers.
\note After this call, the bounding volumes should be updated!
\see Geometry::ComputeBoundingVolumes
*/
FORK_EXPORT void OptimizeGeometryGraph(GeometryPtr& geometry);

/**
Computes the bounding volume for the specified vertex coordinate range.
\param[out] boundingVolume Specifies the resulting bounding volume. The previous information will get lost!
\param[in] coordIterator Specifies the vertex coordinate iterator.
\remarks Usage example:
\code
Scene::GeometryConverter::ComputeBoundingVolume(
    meshGeometry->boundingVolume,
    Video::AttributeConstIterator(&vertices[0].coord, vertices.size(), sizeof(Video::Simple3DVertex))
);
\endcode
\see BoundingVolume
*/
FORK_EXPORT void ComputeBoundingVolume(BoundingVolume& boundingVolume, Video::AttributeConstIterator coordIterator);

/**
Computes the bounding volume for the specified geometry list.
\param[out] boundingVolume Specifies the resulting bounding volume. The previous information will get lost!
\param[in] geometries Specifies the geometry list for which the bounding volume will be computed.
All bounding volumes of the respective geometries will be computed as well.
\see Geometry::ComputeBoundingVolume
*/
FORK_EXPORT void ComputeBoundingVolume(BoundingVolume& boundingVolume, const std::vector<GeometryPtr>& geometries);


/* --- Templates --- */

/**
Flips all normal vectors of the specified geometry.
\tparam Geom Specifies the geometry class. This class must have a member container called "vertices"
whose entries have a member vector called "normal".
This could be "Simple3DMeshGeometry", "TangentSpaceMeshGeometry" or "CommonMeshGeometry" for instance.
\param[in,out] geometry Specifies the geometry object whose vertex normals are to be fliped.
\see Scene::Simple3DMeshGeometry
\see Scene::TangentSpaceMeshGeometry
\see Scene::CommonMeshGeometry
*/
template <class Geom> void FlipNormals(Geom& geometry)
{
    for (auto& vertex : geometry.vertices)
        vertex.normal = -vertex.normal;
}


} // /namespace GeometryConverter

} // /namespace Scene

} // /namespace Fork


#endif



// ========================