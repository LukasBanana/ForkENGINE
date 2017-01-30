/*
 * Editor application file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "EditorApplication.h"
#include "Language/LanguageLoader.h"
#include "Frame/Main/MainFrame.h"
#include "Frame/Console/ConsoleFrame.h"
#include "Engine/EngineInfo.h"
#include "Core/Config.h"
#include "Core/WxHelper.h"
#include "Core/Devices.h"
#include "Core/Config.h"
#include "Core/DefaultMessageBoxes.h"

#if 1//!!!
#   include "Frame/Config/ConfigFrame.h"
#   include "Frame/Main/MainFrameToolBarComposition.h"
#endif

#include <wx/cmdline.h>


namespace Fork
{

namespace Editor
{


EditorApplication* EditorApplication::instance_ = nullptr;

wxIMPLEMENT_APP(EditorApplication);

EditorApplication::EditorApplication()
{
    EditorApplication::instance_ = this;
}
EditorApplication::~EditorApplication()
{
    Devices::CleanUpEngine();
    EditorApplication::instance_ = nullptr;
}

bool EditorApplication::LoadLanguageModule(const std::wstring& libraryName)
{
    languageLibrary_ = Language::LoadLanguageModule(libraryName);
    return languageLibrary_ != nullptr;
}

void EditorApplication::DrawCurrentFrame()
{
    /* Draw all children views of the main frame */
    mainFrame_->DrawChildren();

    /* Reset renderer profiler */
    Devices::renderSystem->ResetProfiler();
}

EditorApplication* EditorApplication::Instance()
{
    return instance_;
}


/*
 * ======= Private: =======
 */

bool EditorApplication::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    /* Keep track of all log messages before terminal is created */
    ConsoleFrame::StartPreLogEventHandler();
    ConsoleFrame::OpenLogFile("log/LogFile");

    /* Load external configurations */
    Config* config = Config::Instance();
    config->LoadConfiguration();

    /* Load language module */
    LoadLanguageModule(L"ForkEdLanguageEN");

    /* Create engine devices and render system */
    Devices::CreateEngine();

    /* Create main editor frame */
    mainFrame_ = MakeWxObject<MainFrame>();

    Config::Instance()->LoadLayout();
    Config::Instance()->LoadSettings();
    
    mainFrame_->Show();

    #if 1//!TEST!
    
    //mainFrame_->GetConfigFrame()->Show();
    //mainFrame_->SwitchRibbonBar(false);

    #endif

    Bind(wxEVT_IDLE, &EditorApplication::OnIdle, this);

    /* Pass program arguments to the console */
    for (int i = 1; i < wxApp::argc; ++i)
    {
        auto arg = wxApp::argv[i];
        mainFrame_->GetConsoleFrame()->GetTerminal()->Execute(arg.ToStdString());
    }

    return true;
}

void EditorApplication::OnInitCmdLine(wxCmdLineParser& parser)
{
    /* Ignore command line parser here */
    parser.SetCmdLine("");
}

bool EditorApplication::OnCmdLineParsed(wxCmdLineParser& parser)
{
    /* Ignore command line parser here */
    return true;
}

void EditorApplication::OnIdle(wxIdleEvent& event)
{
    if (Devices::engine)
    {
        UpdateSystem();
        DrawCurrentFrame();
    
        event.RequestMore();

        Devices::engine->UpdateFrameStates();
    }
}

int EditorApplication::OnExit()
{
    //Clean up ...
    return wxApp::OnExit();
}

static void Rethrow()
{
    try
    {
        throw; // rethrow previous exception
    }
    catch (const DefaultException& err)
    {
        IO::Log::Error(err);
        FatalError(err.FullMessage());
    }
    catch (const std::exception& err)
    {
        IO::Log::Error(err.what());
        FatalError(err.what());
    }
}

bool EditorApplication::OnExceptionInMainLoop()
{
    Rethrow();
    Exit();
    return false;
}

void EditorApplication::OnUnhandledException()
{
    Rethrow();
    Exit();
}

void EditorApplication::UpdateSystem()
{
    /* Process user interaction with all editor views */
    for (auto& view : mainFrame_->GetEditorViews())
        view->ProcessUserInteraction();
}


} // /namespace Editor

} // /namespace Fork



// ========================