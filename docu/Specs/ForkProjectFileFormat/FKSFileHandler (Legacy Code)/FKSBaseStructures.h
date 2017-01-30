/*
 * FKS base structures header
 * 
 * This file is part of the "FKS-FileHandler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FKS_BASE_STRUCTS_H__
#define __FKS_BASE_STRUCTS_H__


#include "FKSBaseTypes.h"


namespace FKS
{


/* --- Floating-point vectors --- */

//! 2D 32-bit float vector.
struct Vector2f
{
    Float32 x = 0.0f, y = 0.0f;
};

//! 3D 32-bit float vector.
struct Vector3f
{
    Float32 x = 0.0f, y = 0.0f, z = 0.0f;
};

//! 4D 32-bit float vector.
struct Vector4f
{
    Float32 x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;
};


/* --- Integer vectors --- */

//! 2D 32-bit integer vector.
struct Vector2i
{
    Int32 x = 0, y = 0;
};

//! 3D 32-bit integer vector.
struct Vector3i
{
    Int32 x = 0, y = 0, z = 0;
};

//! 4D 32-bit integer vector.
struct Vector4i
{
    Int32 x = 0, y = 0, z = 0, w = 0;
};


/* --- Matrices --- */

//! 2x2 32-bit float matrix.
struct Matrix2f
{
    Vector2f x, y;
};

//! 3x3 32-bit float matrix.
struct Matrix3f
{
    Vector3f x, y, z;
};

//! 4x4 32-bit float matrix.
struct Matrix4f
{
    Vector4f x, y, z, w;
};


/* --- Quaternion --- */

//! 32-bit float quaternion.
struct Quaternion
{
    Float32 x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;
};


/* --- Transformations --- */

//! 2D transformation.
struct Transform2Df
{
    Vector2f    position;
    Float32     rotation = 0.0f;
    Vector2f    scale;
};

//! 3D transformation.
struct Transform3Df
{
    Vector3f    position;
    Quaternion  rotation;
    Vector3f    scale;
};


/* --- Graphics configurations --- */

//! Graphics viewport.
struct Viewport
{
    Vector2i    position;
    Vector2i    size;
    Float32     minDepth = 0.0f;
    Float32     maxDepth = 0.0f;
};

//! Graphics projection.
struct Projection
{
    Viewport    viewport;
    Float32     nearPlane   = 0.0f;
    Float32     farPlane    = 0.0f;
    Float32     fieldOfView = 0.0f;
    Boolean     isOrtho     = false;
};

//! Chunk description.
struct ChunkDesc
{
    UInt32 id       = 0; //!< \see ChunkIDList
    UInt32 offset   = 0;
    UInt32 size     = 0;
};


} // /namespace FKS


#endif



// ========================