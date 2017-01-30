/*
 * Simple 3D mesh shader constant buffers header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SIMPLE_3D_MESH_SHADER_CONSTANT_BUFFERS_H__
#define __FORK_SIMPLE_3D_MESH_SHADER_CONSTANT_BUFFERS_H__


#include "Math/Core/Matrix4.h"
#include "Math/Core/Vector4.h"
#include "Video/Core/ColorRGBA.h"


namespace Fork
{

namespace Video
{

namespace StandardShader
{

//! Namespace for all "Simple3DMesh" shader constant buffer structures.
namespace Simple3DMeshShader
{

#include "Core/PackPush.h"


struct VertexConstBuffer
{
    Math::Matrix4f  wvpMatrix;
    Math::Matrix4f  worldMatrix;
    Math::Point3f   viewPosition;
    float           __pad0;
}
PACK_STRUCT;

struct MaterialConstBuffer
{
    Video::ColorRGBAf   ambientColor;
    Video::ColorRGBAf   diffuseColor;
    Video::ColorRGBAf   specularColor;
    Video::ColorRGBAf   emissiveColor;
    float               shininess   = 90.0f;
    int                 numTextures = 0;
    int                 __pad0[2];
}
PACK_STRUCT;

struct LightConstBuffer
{
    static const int maxNumLights = 8;

    struct LightSource
    {
        Math::Vector4f      positionAndType; //!< Position (xyz), type (w).
        Video::ColorRGBAf   colorAndEnergy;  //!< Color (rgb), energy (a).
        Math::Vector4f      direction;       //!< Direction (xyz).
    };

    LightSource lights[maxNumLights];
    int         numLights = 0;
    int         __pad0[3];
}
PACK_STRUCT;


#include "Core/PackPop.h"

} // /namespace Simple3DMeshShader

} // /namespace StandardShader

} // /namespace Video

} // /namespace Fork


#endif



// ========================