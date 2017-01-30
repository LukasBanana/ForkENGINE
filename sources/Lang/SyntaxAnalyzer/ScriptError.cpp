/*
 * Script error file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Lang/SyntaxAnalyzer/ScriptError.h"


namespace Fork
{

namespace Lang
{

namespace SyntaxAnalyzer
{


ScriptError::ScriptError(
    const IO::ErrorTypes type, const std::string& message) :
        IO::Error(type, message)
{
}
ScriptError::ScriptError(
    const IO::ErrorTypes type, const SourcePosition &sourcePos, const std::string &message) :
        IO::Error(type, ScriptError::ConstructMessage(type, sourcePos, message))
{
}
ScriptError::ScriptError(
    const IO::ErrorTypes type, const SourcePosition &sourcePos,
    const std::string &message, const std::string& line, const std::string& marker) :
        IO::Error   (type, ScriptError::ConstructMessage(type, sourcePos, message)  ),
        line_       (line                                                           ),
        marker_     (marker                                                         )
{
    /* Remove new-line character from source line */
    if (line_.size() > 0 && line_.back() == '\n')
        line_.resize(line_.size() - 1);
}
ScriptError::~ScriptError()
{
}

std::string ScriptError::ConstructMessage(
    const IO::ErrorTypes type, const SourcePosition& sourcePos, const std::string& message)
{
    return IO::Error::TypeString(type) + " " + sourcePos.GetString() + " -- " + message;
}


} // /namespace SyntaxAnalyzer

} // /namespace Lang

} // /namespace Fork



// ========================