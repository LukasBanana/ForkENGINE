/*
 * Editor console frame command factory header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_CONSOLE_FRAME_COMMAND_FACTORY_H__
#define __FORK_EDITOR_CONSOLE_FRAME_COMMAND_FACTORY_H__


#include "Utility/Terminal/CommandFactory.h"


namespace Fork
{

namespace Editor
{


class ConsoleFrameCommandFactory : public Utility::CommandFactory
{
    
    public:
        
        ConsoleFrameCommandFactory();

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================