
// ForkENGINE: Script Test
// 28/09/2014

#include <fengine/import.h>

using namespace std;
using namespace Fork;

FORK_IMPLEMENT_APP_MAIN_ONLY(ScriptTest);

void ScriptTest::OnMain()
{
    try
    {
        Script::ScriptInterpreter::EnvironmentDescription envDesc;
        {
            envDesc.scriptEnginePath = L"Python-3.4.2rc1";
            envDesc.scriptModulePath = "scripts/python";
        }
        auto interpreter = engine.CreateScriptInterpreter(L"ForkScriptPy", envDesc);

        auto desc = interpreter->QueryDescriptor();

        interpreter->RunFromFile("TestScript1.py");
    }
    catch (const std::exception& err)
    {
        std::cerr << err.what() << std::endl;
    }

    system("pause");
}

