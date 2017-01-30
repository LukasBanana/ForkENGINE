/*
 * Engine game writer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GameWriter.h"
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

GameWriter::GameWriter(IO::File& file, Game& game) :
    writer_ { file },
    game_   { game }
{
}

bool GameWriter::WriteGame()
{
    /* Print information message */
    IO::Log::Message("Writing game content to file");
    IO::Log::ScopedIndent indent;

    try
    {
        /* Write file header */
        writer_.WriteHeader(gameMagic, formatVersion, formatCompatibleVersion);

        /* Write chunk description list */
        writer_.WriteChunkDescList(ChunkIDList());

        /* Write chunks */
        WriteChunks();

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

void GameWriter::WriteChunks()
{
    //...
}


} // /namespace Format

} // /namespace Engine

} // /namespace Fork



// ========================