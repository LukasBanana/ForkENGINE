/*
 * Python script interpreter file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "PyScriptInterpreter.h"
#include "IO/FileSystem/FileStreamHelper.h"
#include "Module/PyModuleImport.h"


namespace Fork
{

namespace Script
{


PyScriptInterpreter::PyScriptInterpreter(const EnvironmentDescription* environmentDesc)
{
    if (environmentDesc != nullptr && !SetupPythonSearchPath(environmentDesc->scriptEnginePath))
        throw ScriptInterpreterException("Initializing python search path failed");

    ImportAllPythonModules();

    Py_Initialize();

    if (environmentDesc)
    {
        RunFromString(
            "import sys\n"
            "sys.path.append('" + environmentDesc->scriptModulePath + "')\n"
            "import Logger"
        );
    }
}
PyScriptInterpreter::~PyScriptInterpreter()
{
    Py_Finalize();
}

ScriptInterpreter::Descriptor PyScriptInterpreter::QueryDescriptor() const
{
    Descriptor desc;

    desc.language   = "Python";
    desc.fileExt    = "py";
    desc.version    = std::string(Py_GetVersion());
    desc.copyright  = std::string(Py_GetCopyright());

    desc.featureFlags = (FeatureFlags::RunFromString | FeatureFlags::RunFromFile);

    return desc;
}

bool PyScriptInterpreter::RunFromString(const std::string& code)
{
    return PyRun_SimpleString(code.c_str()) == 0;
}

bool PyScriptInterpreter::RunFromFile(const std::string& filename)
{
    std::ifstream stream(filename);
    return RunFromString(IO::ReadFileIntoString(stream));
}


/*
 * ======= Private: =======
 */

bool PyScriptInterpreter::SetupPythonSearchPath(const std::wstring& searchPath)
{
    searchPath_ = searchPath;

    Py_SetPythonHome(&searchPath_[0]);
    Py_SetProgramName(&searchPath_[0]);

    return true;
}


} // /namespace Script

} // /namespace Fork



// ========================