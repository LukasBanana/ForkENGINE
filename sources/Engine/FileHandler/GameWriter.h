/*
 * Engine game writer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENGINE_GAME_WRITER_H__
#define __FORK_ENGINE_GAME_WRITER_H__


#include "../FileHandler/FileWriter.h"


namespace Fork
{

namespace Engine
{

class Game;

namespace Format
{


//! Game content file writer.
class GameWriter
{
    
    public:
        
        GameWriter(IO::File& file, Game& game);

        bool WriteGame();

    private:
        
        void WriteChunks();

        FileWriter  writer_;
        Game&       game_;

};


} // /namespace Format

} // /namespace Engine

} // /namespace Fork


#endif



// ========================