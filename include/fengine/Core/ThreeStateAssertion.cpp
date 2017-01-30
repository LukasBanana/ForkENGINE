/*
 * Three state assertion file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Core/ThreeStateAssertion.h"

#ifdef FORK_DEBUG

#include "Core/Exception/InvalidStateException.h"


namespace Fork
{


FORK_EXPORT void AssertStateBegin(DebugThreeStates& state, const char* procName)
{
    if (state != DebugThreeStates::End)
        throw InvalidStateException(procName, "Invalid state for <Begin> (Required is <End>)");
    state = DebugThreeStates::Begin;
}

FORK_EXPORT void AssertStateUse(DebugThreeStates& state, const char* procName)
{
    if (state == DebugThreeStates::End)
        throw InvalidStateException(procName, "Invalid state for <Use> (Required is <Begin> or <Use>)");
    state = DebugThreeStates::Use;
}

FORK_EXPORT void AssertStateEnd(DebugThreeStates& state, const char* procName)
{
    if (state == DebugThreeStates::End)
        throw InvalidStateException(procName, "Invalid state for <End> (Required is <Begin> or <Use>)");
    state = DebugThreeStates::End;
}


} // /namespace Fork


#endif



// ========================