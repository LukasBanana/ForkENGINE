/*
 * Engine app file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/App.h"
#include "IO/Core/Log.h"
#include "IO/Core/Console.h"


namespace Fork
{

namespace Engine
{


static App* appInstance = nullptr;

App::App()
{
    appInstance = this;
}
App::~App()
{
}

void App::OnInit(int numArgs, char** args)
{
    IO::Log::AddDefaultEventHandler();
    OnInit();
}

void App::OnInit()
{
    // dummy
}

void App::OnMain()
{
    while (!IsQuit())
    {
        OnUpdate();
        OnRender();
        engine.UpdateFrameStates();
    }
}

void App::OnUpdate()
{
    // dummy
}

void App::OnRender()
{
    // dummy
}

void App::OnCleanUp()
{
    // dummy
}

void App::OnUnhandledException(const DefaultException& err)
{
    IO::Log::ClearEventHandlers();
    IO::Log::AddDefaultEventHandler();
    IO::Log::Error(err);
    IO::Console::Wait();
}

void App::OnUnhandledException(const std::exception& err)
{
    IO::Log::ClearEventHandlers();
    IO::Log::AddDefaultEventHandler();
    IO::Log::Error(err.what());
    IO::Console::Wait();
}

void App::OnExit()
{
    // dummy
}

int App::RunApp(App* app, int numArgs, char** args)
{
    if (app)
    {
        try
        {
            app->OnInit(numArgs, args);
            app->OnMain();
            app->OnCleanUp();
        }
        catch (const DefaultException& err)
        {
            app->OnUnhandledException(err);
        }
        catch (const std::exception& err)
        {
            app->OnUnhandledException(err);
        }
        /*catch (...)
        {
            abort();
        }*/
        app->OnExit();
    }
    return 0;
}

App* App::Instance()
{
    return appInstance;
}


/*
 * ======= Protected: =======
 */

bool App::IsQuit()
{
    return true;
}


} // /namespace Engine

} // /namespace Fork



// ========================