/*
 * Engine game reader file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GameReader.h"
#include "Engine/Game/Game.h"
#include "Core/Exception/DefaultException.h"
#include "IO/Core/Log.h"
#include "GameFormat.h"


namespace Fork
{

namespace Engine
{

namespace Format
{


using namespace GameFormat;

GameReader::GameReader(IO::File& file, Game& game) :
    reader_ { file },
    game_   { game }
{
}

bool GameReader::ReadGame()
{
    /* Print information message */
    IO::Log::Message("Reading game content from file");
    IO::Log::ScopedIndent indent;

    try
    {
        /* Read file header */
        reader_.ReadHeader(gameMagic, formatVersion);

        /* Read chunk description list */
        reader_.ReadChunkDescList();
        
        /* Read chunks */
        ReadChunks();

        /* Return with success */
        IO::Log::Success("Completed successful");
        return true;
    }
    catch (const DefaultException& err)
    {
        IO::Log::Error(err);
    }

    /* Return with error */
    return false;
}


/*
 * ======= Private: =======
 */

void GameReader::ReadChunks()
{
    //...
}


} // /namespace Format

} // /namespace Engine

} // /namespace Fork



// ========================