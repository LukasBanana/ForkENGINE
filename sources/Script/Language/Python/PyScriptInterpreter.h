/*
 * Python script interpreter header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PY_SCRIPT_INTERPRETER_H__
#define __FORK_PY_SCRIPT_INTERPRETER_H__


#include "PyScriptCore.h"
#include "Script/ScriptInterpreter.h"


namespace Fork
{

namespace Script
{


//! Python script interpreter implementation.
class PyScriptInterpreter : public ScriptInterpreter
{
    
    public:
        
        PyScriptInterpreter(const EnvironmentDescription* environmentDesc);
        ~PyScriptInterpreter();

        Descriptor QueryDescriptor() const;

        bool RunFromString(const std::string& code);
        bool RunFromFile(const std::string& filename);

    private:
        
        bool SetupPythonSearchPath(const std::wstring& searchPath);

        std::wstring searchPath_;

};


} // /namespace Script

} // /namespace Fork


#endif



// ========================