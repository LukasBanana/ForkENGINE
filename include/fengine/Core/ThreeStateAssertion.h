/*
 * Three state assertion header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_THREE_STATE_ASSERTION_H__
#define __FORK_THREE_STATE_ASSERTION_H__


#include "Core/StaticConfig.h"

#ifdef FORK_DEBUG

#include "Core/Export.h"
#include "Core/Exception/InvalidStateException.h"

#include <string>


#define FORK_DEBUG_3STATE   Fork::DebugThreeStates _debugThreeState_ = Fork::DebugThreeStates::End
#define ASSERT_3STATE_BEGIN Fork::AssertStateBegin(_debugThreeState_, __FUNCTION__)
#define ASSERT_3STATE_USE   Fork::AssertStateUse(_debugThreeState_, __FUNCTION__)
#define ASSERT_3STATE_END   Fork::AssertStateEnd(_debugThreeState_, __FUNCTION__)

#define FORK_DEBUG_3STATE_ID(id)    Fork::DebugThreeStates _debugThreeState_##id = Fork::DebugThreeStates::End
#define ASSERT_3STATE_BEGIN_ID(id)  Fork::AssertStateBegin(_debugThreeState_##id, __FUNCTION__)
#define ASSERT_3STATE_USE_ID(id)    Fork::AssertStateUse(_debugThreeState_##id, __FUNCTION__)
#define ASSERT_3STATE_END_ID(id)    Fork::AssertStateEnd(_debugThreeState_##id, __FUNCTION__)


namespace Fork
{


//! Debugging three state enumeration (only available in debug mode).
enum class DebugThreeStates
{
    Begin,
    Use,
    End,
};

FORK_EXPORT void AssertStateBegin(DebugThreeStates& state, const char* procName);
FORK_EXPORT void AssertStateUse(DebugThreeStates& state, const char* procName);
FORK_EXPORT void AssertStateEnd(DebugThreeStates& state, const char* procName);


} // /namespace Fork


#else


#define FORK_DEBUG_3STATE
#define ASSERT_3STATE_BEGIN
#define ASSERT_3STATE_USE
#define ASSERT_3STATE_END

#define FORK_DEBUG_3STATE_ID(id)
#define ASSERT_3STATE_BEGIN_ID(id)
#define ASSERT_3STATE_USE_ID(id)
#define ASSERT_3STATE_END_ID(id)


#endif

#endif



// ========================