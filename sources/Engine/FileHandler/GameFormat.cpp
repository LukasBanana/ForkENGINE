/*
 * Engine game format file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GameFormat.h"


namespace Fork
{

namespace Engine
{

namespace Format
{

namespace GameFormat
{


std::vector<UInt32> ChunkIDList()
{
    return { ChunkIDs::Scenes, ChunkIDs::Entities, ChunkIDs::Components };
}


} // /namespace MagicNumber

} // /namespace Format

} // /namespace Engine

} // /namespace Fork



// ========================