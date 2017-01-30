/*
 * FKS main structures header
 * 
 * This file is part of the "FKS-FileHandler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FKS_MAIN_STRUCTS_H__
#define __FKS_MAIN_STRUCTS_H__


#include "FKSBaseStructures.h"
#include "FKSChunks.h"

#include <vector>


namespace FKS
{


//! File header.
struct FileHeader
{
    UInt32 magic                    = 0;
    UInt32 version                  = 0;
    UInt32 lastCompatibleVersion    = 0;
};

//! List of chunk descriptions.
struct ChunkDescList
{
    std::vector<ChunkDesc> chunks;
};

//! List of all chunks.
struct ChunkList
{
    /* Configurations */
    ChunkProjections chunkProjections;

    /* Resources */
    ChunkDynamicImages chunkDynamicImages;
    ChunkStaticImages chunkStaticImages;
    ChunkDynamicModels chunkDynamicModels;

    /* Scene nodes */
    ChunkCameraSceneNodes chunkCameraSceneNodes;
};


} // /namespace FKS


#endif



// ========================