/*
 * Geometry generator header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GEOMETRY_GENERATOR_H__
#define __FORK_GEOMETRY_GENERATOR_H__


#include "Scene/Geometry/Node/Simple3DMeshGeometry.h"
#include "Scene/Geometry/Node/TerrainMeshGeometry.h"
#include "Math/Core/MathConstants.h"


namespace Fork
{

namespace Scene
{

/**
Namespace for generators of standard geometries, such as cube, cone, sphere etc.
\note For all of these geometries always the primitive type Video::GeometryPrimitives::Triangles will be used!
Moreover these functions will only generate the geometry data, i.e. set up the "vertices" and "indices",
but they won't upload it to the GPU, i.e. the hardware buffers must be updated by yourself!
\see Video::GeometryPrimitives
\see Simple3DMeshGeometry
*/
namespace GeometryGenerator
{


/* --- Type defintions --- */

//! Standard type for basic geometries.
typedef Simple3DMeshGeometry GeometryType;
typedef std::shared_ptr<GeometryType> GeometryTypePtr;


/* --- Structures --- */

/**
(Wire-) cube geometry description structure.
\see GenerateCube
\see GenerateWireCube
*/
struct FORK_EXPORT CubeDescription
{
    CubeDescription(float radius = 1.0f);

    /**
    Returns true if this description is considered to go before 'other' in a strict weak ordering (SWO).
    This function is used to sort objects of this structure, e.g. in a hash-map.
    \note Strict weak ordering (SWO) is used for sorting.
    Default comparision functions (e.g. "Compare") is used to compare objects of equality.
    */
    bool CompareSWO(const CubeDescription& other) const;

    //! Specifies the cube's radius (or rather radial size).
    float radius = 1.0f;
};

/**
Cone geometry description structure.
\see GenerateCone
*/
struct FORK_EXPORT ConeDescription
{
    ConeDescription(float radius = 1.0f, float height = 2.0f, unsigned int segments = 32);

    //! \see CubeDescription::CompareSWO
    bool CompareSWO(const ConeDescription& other) const;

    //! Specifies the cone radius. By default 1.0.
    float           radius      = 1.0f;

    //! Specifies the cone height. By default 1.0.
    float           height      = 1.0f;

    /**
    Specifies the number of segmentation. The minimal value for this is 3.
    By default 32, i.e. the circle of the cone's bottom will have 32 edges.
    */
    unsigned int    segments    = 32;
};

/**
Cylinder geometry description structure.
\see GenerateCylinder
*/
struct FORK_EXPORT CylinderDescription
{
    CylinderDescription(float radius = 1.0f, float height = 2.0f, unsigned int segments = 32);

    //! \see CubeDescription::CompareSWO
    bool CompareSWO(const CylinderDescription& other) const;

    //! Specifies the cylinder radius. By default 1.0.
    float           radius      = 1.0f;

    //! Specifies the cylinder height. By default 2.0.
    float           height      = 2.0f;

    /**
    Specifies the number of segmentation. The minimal value for this is 3.
    By default 32, i.e. the circle of the cylinder's bottom will have 32 edges.
    */
    unsigned int    segments    = 32;
};

/**
UV-sphere geometry description structure.
\see GenerateUVSphere
*/
struct FORK_EXPORT UVSphereDescription
{
    UVSphereDescription(float radius, unsigned int segmentsU, unsigned int segmentsV);
    /**
    This is equivalent to:
    \code
    UVSphereDescription(radius, segments*2, segments)
    \endcode
    */
    UVSphereDescription(float radius = 1.0f, unsigned int segments = 32);

    //! \see CubeDescription::CompareSWO
    bool CompareSWO(const UVSphereDescription& other) const;

    //! Specifies the sphere radius.
    float           radius      = 1.0f;
    
    //! Specifies the number of segmentation in U direction. The minimal value for this is 3.
    unsigned int    segmentsU   = 64;

    //! Specifies the number of segmentation in V direction. The minimal value for this is 2.
    unsigned int    segmentsV   = 32;
};

/**
Ico-sphere geometry description structure.
\see GenerateIcoSphere
*/
struct FORK_EXPORT IcoSphereDescription
{
    IcoSphereDescription(float radius = 1.0f, unsigned int tessellation = 8);

    //! \see CubeDescription::CompareSWO
    bool CompareSWO(const IcoSphereDescription& other) const;

    //! Specifies the sphere radius.
    float           radius          = 1.0f;

    //! Specifies the number of subdivision (or rather tessellation factor).
    unsigned int    tessellation    = 8;
};

/**
Torus geometry description structure.
\see GenerateTorus
*/
struct FORK_EXPORT TorusDescription
{
    TorusDescription(
        float ringRadius, float innerRadiusU, float innerRadiusV,
        unsigned int segmentsU, unsigned int segmentsV
    );
    TorusDescription(
        float ringRadius = 1.0f, float innerRadius = 0.5f, unsigned int segments = 32
    );

    //! \see CubeDescription::CompareSWO
    bool CompareSWO(const TorusDescription& other) const;

    //! Specifies the radius of the ring.
    float           ringRadius      = 1.0f;

    //! Specifies the inner radius in U direction.
    float           innerRadiusU    = 0.5f;

    //! Specifies the inner radius in V direction.
    float           innerRadiusV    = 0.5f;

    //! Specifies the number of segmentation in U direction. The minimal value for this is 3.
    unsigned int    segmentsU       = 32;

    //! Specifies the number of segmentation in V direction. The minimal value for this is 2.
    unsigned int    segmentsV       = 32;
};

/**
Spiral geometry description structure.
\see GenerateSpiral
*/
struct FORK_EXPORT SpiralDescription
{
    SpiralDescription(
        float ringRadius,
        float innerRadiusU, float innerRadiusV,
        float offset, float rotation,
        unsigned int segmentsU, unsigned int segmentsV
    );
    SpiralDescription(
        float ringRadius = 1.0f, float innerRadius = 0.5f,
        float offset = 1.0f, float rotation = Math::pi * 2.0f,
        unsigned int segments = 32
    );

    //! \see CubeDescription::CompareSWO
    bool CompareSWO(const SpiralDescription& other) const;

    //! Specifies the radius of the ring.
    float           ringRadius      = 1.0f;
    
    //! Specifies the inner radius in U direction.
    float           innerRadiusU    = 0.5f;
    
    //! Specifies the inner radius in V direction.
    float           innerRadiusV    = 0.5f;
    
    //! Specifies the offset within each rotation.
    float           offset          = 1.0f;

    /**
    Specifies the rotation (in radians) for the whole spiral.
    If the spiral is to be rotated twice, set this to (PI*4); if the spiral is to
    be rotated around 400 degrees, set this to (Math::deg2rad * 400.0f).
    */
    float           rotation        = Math::pi * 2.0f;

    //! Specifies the number of segmentation in U direction. The minimal value for this is 3.
    unsigned int    segmentsU       = 32;

    //! Specifies the number of segmentation in V direction. The minimal value for this is 2.
    unsigned int    segmentsV       = 32;
};

/**
Capsule geometry description structure.
\see GenerateCapsule
*/
struct FORK_EXPORT CapsuleDescription
{
    CapsuleDescription(
        float radius, float height, unsigned int segmentsU, unsigned int segmentsV
    );
    CapsuleDescription(
        float radius = 1.0f, float height = 1.0f, unsigned int segments = 32
    );

    //! \see CubeDescription::CompareSWO
    bool CompareSWO(const CapsuleDescription& other) const;

    //! Specifies the sphere radius.
    float           radius      = 1.0f;

    /**
    Specifies the cylinder height. The height of the complete capsule is (height + radius*2).
    If you set this to 0.0, the geometry looks like a UV-sphere.
    */
    float           height      = 1.0f;

    //! Specifies the number of segmentation in U direction. The minimal value for this is 3.
    unsigned int    segmentsU   = 32;

    //! Specifies the number of segmentation in V direction. The minimal value for this is 2.
    unsigned int    segmentsV   = 32;
};


/* --- Geometry primitives --- */

//! Generates geometry for a cube mesh.
FORK_EXPORT void GenerateCube(GeometryType& geometry, const CubeDescription& desc = CubeDescription());

/**
Generates wire geometry for a cube mesh. The primitive type for this is Video::GeometryPrimitives::Lines.
\note This function only generates 3D coordinates for the each vertex!
*/
FORK_EXPORT void GenerateWireCube(GeometryType& geometry, const CubeDescription& desc = CubeDescription());

//! Generates geometry for a cone mesh.
FORK_EXPORT void GenerateCone(GeometryType& geometry, const ConeDescription& desc = ConeDescription());

//! Generates geometry for the cylinder mesh.
FORK_EXPORT void GenerateCylinder(GeometryType& geometry, const CylinderDescription& desc = CylinderDescription());

/**
Generates geometry for the UV-sphere mesh. A UV-sphere will have large triangles
at its equator but small triangles at their north- and south poles. To avoid this, use the ico-sphere.
*/
FORK_EXPORT void GenerateUVSphere(GeometryType& geometry, const UVSphereDescription& desc = UVSphereDescription());

/**
Generates geometry for the ico-sphere mesh. An ico-sphere is derived
from the icosahedron, where each edge and each area has the same size.
*/
FORK_EXPORT void GenerateIcoSphere(GeometryType& geometry, const IcoSphereDescription& desc = IcoSphereDescription());

//! Generates geometry for the torus mesh (also called "ring").
FORK_EXPORT void GenerateTorus(GeometryType& geometry, const TorusDescription& desc = TorusDescription());

//! Generates geometry for the spiral mesh.
FORK_EXPORT void GenerateSpiral(GeometryType& geometry, const SpiralDescription& desc = SpiralDescription());

//! Generates geometry for the capsule mesh.
FORK_EXPORT void GenerateCapsule(GeometryType& geometry, const CapsuleDescription& desc = CapsuleDescription());

/* --- Extended geometries --- */

/**
Generates the template geometry for a terrain.
\param[out] geometry Specifies the resulting terrain mesh geometry.
\param[in] gridSize Specifies the grid size of the terrain template. If this is 1,
the geometry only consists of a single quad (or rather two triangles). This must not be 0!
\throws InvalidArgumentException If 'gridSize' is 0.
\see TerrainGeometry
\see TerrainMeshGeometry
*/
FORK_EXPORT void GenerateTerrainTemplate(TerrainMeshGeometry& geometry, unsigned int gridSize);


} // /CubeGeneratorGenerator

} // /namespace Scene

} // /namespace Fork


#endif



// ========================