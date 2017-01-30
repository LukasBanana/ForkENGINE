/*
 * Component exception file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/Component/ComponentException.h"


namespace Fork
{


ComponentException::ComponentException(const std::string& procName, const Failures failure) :
    DefaultException
    {
        "ComponentException",
        "Function: " + procName + ", Description: " + ComponentException::FailureToString(failure)
    },
    failure_{ failure }
{
}

std::string ComponentException::FailureToString(const Failures failure)
{
    switch (failure)
    {
        case Failures::ComponentAlreadyExists:
            return "Component already exists";
        case Failures::ComponentIncompatible:
            return "Component is incompatible with other components";
        default:
            break;
    }
    return "<Unknown>";
}


} // /namespace Fork



// ========================