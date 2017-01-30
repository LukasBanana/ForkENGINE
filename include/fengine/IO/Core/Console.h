/*
 * Console header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_CONSOLE_H__
#define __FORK_IO_CONSOLE_H__


#include "Core/Export.h"
#include "IO/Core/Log.h"

#include <string>


namespace Fork
{

namespace IO
{

//! Console namespace for basic text output.
namespace Console
{


//! Default log event handler which uses the console.
class FORK_EXPORT LogEventHandler : public Log::EventHandler
{
    
    public:
        
        void OnPrint(const std::string& message, const Log::EntryTypes type) override;

        void OnStartLn(const std::string& indent) override;
        void OnEndLn() override;

        void OnPushColor(const Log::ColorFlags& frontColorFlags) override;
        void OnPushColor(const Log::ColorFlags& frontColorFlags, const Log::ColorFlags& backColorFlags) override;
        void OnPopColor() override;

        void OnBlank() override;

};


//! Prints the specified text to the standard output.
FORK_EXPORT void Print(const std::string& text);
//! Prints the specified text as new line to the standard output.
FORK_EXPORT void PrintLn(const std::string& text);
//! Prints a line ending (std::endl).
FORK_EXPORT void PrintLn();

//! Waits for any user input (pauses the console).
FORK_EXPORT void Wait();


} // /namespace Console

} // /namespace IO

} // /namespace Fork


#endif



// ========================