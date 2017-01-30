/*
 * Error file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/Core/Error.h"


namespace Fork
{

namespace IO
{


Error::Error(const ErrorTypes type, const std::string& message) :
    DefaultException
    {
        Error::TypeString(type),
        message
    },
    type_(type)
{
}
Error::~Error()
{
}

std::string Error::TypeString(const ErrorTypes type)
{
    switch (type)
    {
        case ErrorTypes::Default:
            return "Error";
        case ErrorTypes::Fatal:
            return "Fatal Error";
        case ErrorTypes::Syntax:
            return "Syntax Error";
        case ErrorTypes::Context:
            return "Context Error";
    }
    return "";
}


} // /namespace IO

} // /namespace Fork



// ========================