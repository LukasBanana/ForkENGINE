/*
 * Editor console frame file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ConsoleFrame.h"
#include "ConsoleFrameCommandFactory.h"
#include "../../Core/UIExtensions/UIScreenFrame.h"
#include "../../Language/LanguageLoader.h"
#include "../../Core/StaticLayoutConfig.h"
#include "../../Core/WxHelper.h"
#include "../../Core/Devices.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Core/StringModifier.h"
#include "IO/Core/Console.h"
#include "IO/FileSystem/FileOpenException.h"
#include "IO/Core/SystemClock.h"
#include "Core/Exception/NullPointerException.h"


namespace Fork
{

namespace Editor
{


using namespace StaticLayout::ConsoleFrameLayout;

std::shared_ptr< ConsoleFrame::PreLogEventHandler  > ConsoleFrame::preLogEventHandler_;
std::shared_ptr< ConsoleFrame::LogFileEventHandler > ConsoleFrame::logFileEventHandler_;

ConsoleFrame::ConsoleFrame(wxWindow* parent) :
    EditorFrame{ parent, LANG_STRING(Terminal), defaultSize, minSize }
{
    BuildFrameLayout();

    CreateTerminal();
    CreateEventHandler();

    statusBar_ = CreateStatusBar(4);
    UpdateStatusText();
}
ConsoleFrame::~ConsoleFrame()
{
    IO::Log::RemoveEventHandler(logEventHandler_);
}

void ConsoleFrame::OnFocus(wxFocusEvent& event)
{
    terminal_->Enable(true);
}

void ConsoleFrame::OnLostFocus(wxFocusEvent& event)
{
    terminal_->Enable(false);
}

void ConsoleFrame::OnMove(wxMoveEvent& event)
{
    Draw();
}

void ConsoleFrame::Draw()
{
    if (!screenFrame_)
        return;

    auto renderContext = screenFrame_->GetRenderContext();
    auto renderSystem = Video::RenderSystem::Active();

    UpdateStatusText();

    /*
    Just activate the screen. No need here to clear the buffers,
    because depth test is disabled and the whole screen will be drawn.
    */
    renderContext->Activate();
    {
        renderContext->SetupViewport({ { 0, 0 }, renderContext->GetVideoMode().resolution.Cast<int>() });

        Devices::primitiveRenderer->BeginDrawing2D();
        {
            terminal_->GetView()->groundColor.a = 1.0f;
            terminal_->Draw(terminal_->IsEnabled());
        }
        Devices::primitiveRenderer->EndDrawing2D();
    }
    renderContext->Present();
}

void ConsoleFrame::StartPreLogEventHandler()
{
    if (!preLogEventHandler_)
    {
        preLogEventHandler_ = std::make_shared<PreLogEventHandler>();
        IO::Log::AddEventHandler(preLogEventHandler_);
    }
}

bool ConsoleFrame::OpenLogFile(const std::string& filename)
{
    if (logFileEventHandler_)
        return false;

    try
    {
        /* Add log-file event handler to log output */
        logFileEventHandler_ = std::make_shared<LogFileEventHandler>(filename);
        IO::Log::AddEventHandler(logFileEventHandler_);
    }
    catch (const FileOpenException& err)
    {
        IO::Log::Error(err);
        return false;
    }

    return true;
}

bool ConsoleFrame::IsErrorMessage(const std::string& message)
{
    return message.compare(0, 5, "Error") == 0;
}


/*
 * ======= Private: =======
 */

bool ConsoleFrame::BuildFrameLayout()
{
    screenFrame_ = MakeWxObject<UIScreenFrame>(this);

    if (!UIScreenFrame::CreateRenderContext(screenFrame_))
    {
        screenFrame_->Destroy();
        screenFrame_ = nullptr;
        return false;
    }

    /* Bind screen frame to focus callbacks to enable and disable terminal user input */
    screenFrame_->Bind(wxEVT_SET_FOCUS,     &ConsoleFrame::OnFocus,     this);
    screenFrame_->Bind(wxEVT_KILL_FOCUS,    &ConsoleFrame::OnLostFocus, this);

    screenFrame_->SetResizeCallback([&](const Math::Size2i&){ Draw(); });

    Bind(wxEVT_MOVE, &ConsoleFrame::OnMove, this);

    return true;
}

void ConsoleFrame::CreateTerminal()
{
    /* Create terminal utility object */
    terminal_ = std::make_unique<Utility::Terminal>(
        Devices::primitiveRenderer,
        std::make_shared<ConsoleFrameCommandFactory>()
    );
    terminal_->Message("Enter \"help\" to show the man page");

    /* Connect script event handler with script interpreter */
    terminal_->SetScriptEventHandler(
        std::make_shared<TerminalScriptEventHandler>(Devices::scriptInterpreter.get())
    );

    /* Initialize default appearance */
    terminal_->GetView()->groundColor = { 0, 0, 0.2f };
}

void ConsoleFrame::CreateEventHandler()
{
    /* Create event handler to transmit log message */
    logEventHandler_ = std::make_shared<LogEventHandler>(terminal_.get());
    IO::Log::AddEventHandler(logEventHandler_);

    /* Flush pre-log event handler */
    IO::Log::RemoveEventHandler(ConsoleFrame::preLogEventHandler_);
    ConsoleFrame::preLogEventHandler_->Flush(*terminal_);
    ConsoleFrame::preLogEventHandler_ = nullptr;
}

void ConsoleFrame::UpdateStatusText()
{
    if (statusBar_)
    {
        auto termModel = terminal_->GetModel();
    
        /* Setup text0 string */
        wxString text0 = "Col ";
        text0 += ToStr(termModel->GetCursorPosition() + 1);

        statusBar_->SetStatusText(text0, 0);

        /* Setup text1 string */
        wxString text1 = (termModel->insertionEnabled ? "OVR" : "INS");
        statusBar_->SetStatusText(text1, 1);
    }
}


/*
 * BaseLogEventHandler class
 */

void ConsoleFrame::BaseLogEventHandler::OnPushColor(const ColorFlags& frontColorFlags)
{
    colorStack_.push(currentColor_);
    currentColor_ = ToColor(frontColorFlags);
}

void ConsoleFrame::BaseLogEventHandler::OnPushColor(const ColorFlags& frontColorFlags, const ColorFlags& backColorFlags)
{
    OnPushColor(frontColorFlags);
}

void ConsoleFrame::BaseLogEventHandler::OnPopColor()
{
    if (!colorStack_.empty())
    {
        currentColor_ = colorStack_.top();
        colorStack_.pop();
    }
    else
        currentColor_ = {};
}


/*
 * PreLogEventHandler class
 */

void ConsoleFrame::PreLogEventHandler::OnPrintLn(
    const std::string& indent, const std::string& message, const IO::Log::EntryTypes type)
{
    messages_.push_back({ indent + message, GetCurrentColor() });

    if (ConsoleFrame::IsErrorMessage(message))
        Devices::errorReporter.RegisterError(message);
}

void ConsoleFrame::PreLogEventHandler::Flush(Utility::Terminal& terminal)
{
    for (const auto& msg : messages_)
        terminal.Message(msg.text, msg.color);
    messages_.clear();
}


/*
 * LogEventHandler class
 */

ConsoleFrame::LogEventHandler::LogEventHandler(Utility::Terminal* terminal) :
    terminal_(terminal)
{
}

void ConsoleFrame::LogEventHandler::OnPrint(const std::string& message, const IO::Log::EntryTypes type)
{
    terminal_->Print(message, GetCurrentColor());
    if (ConsoleFrame::IsErrorMessage(message))
        Devices::errorReporter.RegisterError(message);
}

void ConsoleFrame::LogEventHandler::OnStartLn(const std::string& indent)
{
    terminal_->Print(indent, GetCurrentColor());
}

void ConsoleFrame::LogEventHandler::OnEndLn()
{
    terminal_->Blank();
}


/*
 * LogFileEventHandler class
 */

// Returns the default log HTML file document type
static IO::LogFile::LogHTMLEventHandler::DocumentType GetLogHTMLFileDocType()
{
    IO::LogFile::LogHTMLEventHandler::DocumentType docType;

    docType.insertImages    = false;//true;
    docType.clampImageSize  = 128;

    return docType;
}

ConsoleFrame::LogFileEventHandler::LogFileEventHandler(const std::string& filename) :
    IO::LogFile::LogHTMLEventHandler(filename + ".html", "ForkEd Debug Log", GetLogHTMLFileDocType())
{
    OnPrintLn("", "ForkEd Log File", IO::Log::EntryTypes::Info);
    OnPrintLn("", "[ " + IO::SystemClock::CurrentTimePoint() + " ]", IO::Log::EntryTypes::Info);
    OnPrintLn("", "", IO::Log::EntryTypes::Info);
}


/*
 * TerminalScriptEventHandler class
 */

ConsoleFrame::TerminalScriptEventHandler::TerminalScriptEventHandler(Script::ScriptInterpreter* interpreter) :
    interpreter_{ interpreter }
{
}

void ConsoleFrame::TerminalScriptEventHandler::OnRunCode(const std::string& code)
{
    if (interpreter_)
        interpreter_->RunFromString(code);
    else
        IO::Log::Error("Script interpreter not available");
}


} // /namespace Editor

} // /namespace Fork



// ========================