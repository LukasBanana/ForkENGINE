/*
 * Script interpreter file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Script/ScriptInterpreter.h"
#include "Platform/Core/OS.h"


namespace Fork
{

namespace Script
{


ScriptInterpreter::ScriptInterpreter()
{
}
ScriptInterpreter::~ScriptInterpreter()
{
}

ScriptInterpreterPtr ScriptInterpreter::Create(const Platform::DynamicLibraryPtr& library, const EnvironmentDescription& environmentDesc)
{
    const std::string procName = "AllocScriptInterpreter";

    /* Load script interpreter allocation procedure from library */
    DEFINE_PROC_INTERFACE(void*, PFNALLOCSCRIPTINTERPRETERPROC, (const void*));
    auto AllocScriptInterpreter = reinterpret_cast<PFNALLOCSCRIPTINTERPRETERPROC>(library->LoadProc(procName));

    if (!AllocScriptInterpreter)
    {
        throw ScriptInterpreterException(
            "Procedure \"" + procName + "\" not found in scripting library \"" + ToStr(library->GetName()) + "\""
        );
    }

    /* Allocate script interpreter via library function */
    auto scriptInterpreter = reinterpret_cast<ScriptInterpreter*>(
        AllocScriptInterpreter(reinterpret_cast<const void*>(&environmentDesc))
    );

    if (!scriptInterpreter)
    {
        throw ScriptInterpreterException(
            "Allocating script interpreter from library \"" + ToStr(library->GetName()) + "\" failed"
        );
    }

    return std::shared_ptr<ScriptInterpreter>(scriptInterpreter);
}

bool ScriptInterpreter::RunFromString(const std::string& code)
{
    return false;
}

bool ScriptInterpreter::RunFromFile(const std::string& filename)
{
    return false;
}


} // /namespace Physics

} // /namespace Fork



// ========================