/*
 * Script library interface header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SCRIPT_LIB_INTERFACE_H__
#define __FORK_SCRIPT_LIB_INTERFACE_H__


#include "Core/Export.h"


extern "C"
{

/**
Returns a raw-pointer to the allocated script interpreter.
This should be wrapped into a smart pointer:
\param[in] environmentDesc Raw pointer to the 'ScriptInterpreter::EnvironmentDescription' structure.
\code
Script::ScriptInterpreter::EnvironmentDescription environmentDesc;
// environmentDesc ...
auto physicsSystem = std::shared_ptr<ScriptInterpreter>(
    reinterpret_cast<Script::ScriptInterpreter*>(AllocScriptInterpreter(&environmentDesc))
);
\endcode
\see Fork::Script::ScriptInterpreter::EnvironmentDescription
*/
FORK_EXPORT void* AllocScriptInterpreter(const void* environmentDesc);

}


#endif



// ========================