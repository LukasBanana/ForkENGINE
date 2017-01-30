/*
 * FKS chunks header
 * 
 * This file is part of the "FKS-FileHandler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FKS_CHUNKS_H__
#define __FKS_CHUNKS_H__


#include "FKSBaseStructures.h"

#include <vector>
#include <string>


namespace FKS
{


/* --- Global enumerations --- */

enum class ChunkIDList : UInt32
{
    /* Configurations */
    Projections         = 0x00000001,

    /* Resources */
    DynamicImages       = 0x00000010,
    StaticImages        = 0x00000011,
    DynamicModels       = 0x00000012,
    StaticModels        = 0x00000013,

    /* Scene nodes */
    CameraSceneNodes    = 0x00000100,

    /* Graphics objects */
    BlendStates         = 0x00001000,
    RasterizerStates    = 0x00001001,
    DepthStencilStates  = 0x00001002,
    Textures            = 0x00001003,
    Shaders             = 0x00001004,
    ShaderCompositions  = 0x00001005,
};


/* --- Chunks (Configurations) --- */

struct ChunkProjections
{
    std::vector<Projection> projections;
};


/* --- Chunks (Resources) --- */

struct ChunkDynamicImages
{
    struct DynamicImage
    {
        struct Flags
        {
            typedef UInt32 DataType;
            enum : DataType
            {
                None            = 0,
                ConvertToUByte  = (1 << 0),
                ConvertToFloat  = (1 << 1),
            };
        };

        Flags::DataType flags               = Flags::None;
        std::string     relativeFilename;
    };

    std::vector<DynamicImage> images;
};

struct ChunkStaticImages
{
    struct StaticImage
    {
        enum class ImageTypes : UInt8
        {
            UByte = 0,
            Float = 1,
        };

        ImageTypes              type            = ImageTypes::UByte;
        UInt8                   numComponents   = 0;

        std::vector<UInt8>      imageDataUByte;
        std::vector<Float32>    imageDataFloat;
    };

    std::vector<StaticImage> images;
};

struct ChunkDynamicModels
{
    struct DynamicModel
    {
        struct Flags
        {
            typedef UInt32 DataType;
            enum : DataType
            {
                None                = 0,
                CalcNormals         = (1 << 0),
                CalcTangentSpace    = (1 << 1),
            };
        };

        Flags::DataType flags               = Flags::None;
        std::string     relativeFilename;
    };

    std::vector<DynamicModel> models;
};


/* --- Chunks (Scene nodes) --- */

struct ChunkCameraSceneNodes
{
    struct CameraSceneNode
    {
        Transform3Df    transform;
        Projection*     projection = nullptr;
    };

    std::vector<CameraSceneNode> cameras;
};


} // /namespace FKS


#endif



// ========================