/*
 * Standard command listing header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_STANDARD_COMMAND_LISTING_H__
#define __FORK_UTILITY_STANDARD_COMMAND_LISTING_H__


#include "Utility/Terminal/Command.h"


namespace Fork
{

namespace Utility
{


#define DeclStdCmd(n)                                                   \
    class StdCmd##n : public Command                                    \
    {                                                                   \
                                                                        \
        public:                                                         \
                                                                        \
            std::string Name() const;                                   \
            std::string Help() const;                                   \
                                                                        \
            bool Execute(                                               \
                const IO::CommandLine::ArgumentListType& args,          \
                Terminal* terminal = nullptr, void* userData = nullptr  \
            );                                                          \
                                                                        \
    }

DeclStdCmd( Bg      );
DeclStdCmd( Clear   );
DeclStdCmd( Exit    );
DeclStdCmd( Font    );
DeclStdCmd( Help    );
DeclStdCmd( Script  );
DeclStdCmd( Term    );
DeclStdCmd( Version );

#undef DeclStdCmd


} // /namespace Utility

} // /namespace Fork


#endif



// ========================