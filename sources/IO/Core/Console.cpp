/*
 * Console file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/Core/Console.h"
#include "Platform/Core/ConsoleManip.h"

#include <iostream>


namespace Fork
{

namespace IO
{

//! Console namespace for basic text output.
namespace Console
{


using namespace Platform::ConsoleManip;

/*
 * LogEventHandler class
 */

void LogEventHandler::OnPrint(const std::string& message, const Log::EntryTypes type)
{
    std::cout << message;
}

void LogEventHandler::OnStartLn(const std::string& indent)
{
    std::cout << indent;
}

void LogEventHandler::OnEndLn()
{
    std::cout << std::endl;
}

void LogEventHandler::OnPushColor(const Log::ColorFlags& frontColorFlags)
{
    PushAttrib();
    ChangeColor(frontColorFlags);
}

void LogEventHandler::OnPushColor(const Log::ColorFlags& frontColorFlags, const Log::ColorFlags& backColorFlags)
{
    PushAttrib();
    ChangeColor(frontColorFlags, backColorFlags);
}

void LogEventHandler::OnPopColor()
{
    PopAttrib();
}

void LogEventHandler::OnBlank()
{
    std::cout << std::endl;
}


/* --- Global functions --- */

FORK_EXPORT void Print(const std::string& text)
{
    std::cout << text;
}

FORK_EXPORT void PrintLn(const std::string& text)
{
    std::cout << text << std::endl;
}

FORK_EXPORT void PrintLn()
{
    std::cout << std::endl;
}

FORK_EXPORT void Wait()
{
    //todo -> make this OS specific!!!
    system("pause");
}


} // /namespace Console

} // /namespace IO

} // /namespace Fork



// ========================