/*
 * Engine game reader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENGINE_GAME_READER_H__
#define __FORK_ENGINE_GAME_READER_H__


#include "../FileHandler/FileReader.h"


namespace Fork
{

namespace Engine
{

class Game;

namespace Format
{


//! Game content file reader.
class GameReader
{
    
    public:
        
        GameReader(IO::File& file, Game& game);

        bool ReadGame();

    private:

        void ReadChunks();
        
        FileReader  reader_;
        Game&       game_;

};


} // /namespace Format

} // /namespace Engine

} // /namespace Fork


#endif



// ========================