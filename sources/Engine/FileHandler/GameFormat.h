/*
 * Engine game format header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENGINE_GAME_FORMAT_H__
#define __FORK_ENGINE_GAME_FORMAT_H__


#include "BaseTypes.h"

#include <string>
#include <vector>


namespace Fork
{

namespace Engine
{

namespace Format
{

namespace GameFormat
{


static const std::string gameMagic  = "fkgm";
static const std::string sceneMagic = "fksc";
static const std::string assetMagic = "fkas";
static const std::string stateMagic = "fkst";

static UInt16 formatVersion             = 100;
static UInt16 formatCompatibleVersion   = 100;


//! Enumeration of all chunk IDs of the game format.
struct ChunkIDs
{
    enum : UInt32
    {
        Scenes = 1, //!< Chunk of all game scenes.
        Entities,   //!< Chunk of all entities.
        Components, //!< Chunk of all entity components.
    };
};


//! Returns the list of all chunk IDs.
std::vector<UInt32> ChunkIDList();


} // /namespace MagicNumber

} // /namespace Format

} // /namespace Engine

} // /namespace Fork


#endif



// ========================