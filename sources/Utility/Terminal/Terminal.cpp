/*
 * Terminal file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/Terminal/Terminal.h"
#include "Core/Exception/NullPointerException.h"
#include "Video/RenderSystem/RenderContext.h"
#include "Platform/Core/Clipboard.h"

#include <algorithm>


namespace Fork
{

namespace Utility
{


/*
 * LogEventHandler class
 */

Terminal::LogEventHandler::LogEventHandler(Terminal* terminal) :
    terminal_{ terminal }
{
    ASSERT_POINTER(terminal);
}

void Terminal::LogEventHandler::OnPrintLn(const std::string& indent, const std::string& message, const IO::Log::EntryTypes type)
{
    terminal_->Message(indent + message, ActiveColor());
}

void Terminal::LogEventHandler::OnPrint(const std::string& message, const IO::Log::EntryTypes type)
{
    terminal_->Print(message, ActiveColor());
}

Video::ColorRGBf Terminal::LogEventHandler::ActiveColor() const
{
    return colorStack.empty() ?
        Video::ColorRGBf(1.0f) :
        ToColor(colorStack.top() | Platform::ConsoleManip::Colors::Intens);
}

void Terminal::LogEventHandler::OnStartLn(const std::string& indent)
{
    terminal_->Print(indent);
}

void Terminal::LogEventHandler::OnEndLn()
{
    terminal_->Blank();
}

void Terminal::LogEventHandler::OnPushColor(const IO::Log::ColorFlags& frontColorFlags)
{
    colorStack.push(frontColorFlags);
}

void Terminal::LogEventHandler::OnPushColor(const IO::Log::ColorFlags& frontColorFlags, const IO::Log::ColorFlags& backColorFlags)
{
    colorStack.push(frontColorFlags);
}

void Terminal::LogEventHandler::OnPopColor()
{
    if (!colorStack.empty())
        colorStack.pop();
}

void Terminal::LogEventHandler::OnBlank()
{
    terminal_->Blank();
}


/*
 * Terminal class
 */

Terminal::Terminal(
    const Video::PrimitiveRendererPtr& renderer, const CommandFactoryPtr& commandFactory,
    const TerminalModelPtr& model, const TerminalViewPtr& view, const Video::TextureFontPtr& defaultFont) :
        renderer_       { renderer       },
        commandFactory_ { commandFactory },
        model_          { model          },
        view_           { view           },
        defaultFont_    { defaultFont    }
{
    ASSERT_POINTER(renderer);

    /* Create font objects */
    if (!defaultFont_)
    {
        static const int fontHeight = 15;
        defaultFont_ = renderer_->CreateTextureFont({ /*"courier new"*/"consolas", fontHeight });
    }

    /* Setup terminal command executor, model and view */
    if (!commandFactory_)
        commandFactory_ = std::make_shared<CommandFactory>();
    if (!model_)
        model_ = std::make_shared<TerminalModel>();
    if (!view_)
        view_ = std::make_shared<TerminalView>(renderer_.get(), GetModel(), defaultFont_.get());

    /* Add keyboard event handler for typing */
    keyboardEventHandler_ = std::make_shared<KeyboardEventHandler>(this);
    IO::Keyboard::Instance()->AddEventHandler(keyboardEventHandler_);

    /* Add mouse event handler for scrolling */
    mouseEventHandler_ = std::make_shared<MouseEventHandler>(this);
    IO::Mouse::Instance()->AddEventHandler(mouseEventHandler_);
}
Terminal::~Terminal()
{
    /* Remove event handlers */
    IO::Keyboard::Instance()->RemoveEventHandler(keyboardEventHandler_);
    IO::Mouse::Instance()->RemoveEventHandler(mouseEventHandler_);
}

/*
Don't add an optimization like "if (isEnabled_ == isEnabled) { return true; }",
because maybe there are several terminal instances.
But only one terminal will have the keyboard- and mouse foucs!
*/
bool Terminal::Enable(bool isEnabled)
{
    if (isEnabled)
    {
        /* Lock keyboard focus */
        if (!keyboardEventHandler_->LockFocus())
        {
            isEnabled_ = false;
            return false;
        }

        /* Lock mouse focus */
        if (!mouseEventHandler_->LockFocus())
        {
            /* If failed, unlock previously locked keyboard focus */
            keyboardEventHandler_->UnlockFocus();
            isEnabled_ = false;
            return false;
        }
    }
    else
    {
        if ( !keyboardEventHandler_ ->UnlockFocus() ||
             !mouseEventHandler_    ->UnlockFocus() )
        {
            isEnabled_ = false;
            return false;
        }
    }

    /* Store current state */
    isEnabled_ = isEnabled;

    return true;
}

void Terminal::Print(
    const std::wstring& text, const Video::ColorRGBf& color, const Video::TextureFont* font)
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (!font)
        font = defaultFont_.get();

    auto AddEntry = [&](const std::wstring& subText, bool indicateNewLine)
    {
        /* Create new text entry */
        auto entry = std::make_shared<TerminalModel::TextEntry>(
            font, subText, color, indicateNewLine
        );
        GetModel()->AddEntry(entry);
    };

    /* Add entries for each text line */
    size_t start = 0, end = 0;
    while ( ( end = text.find('\n', start) ) != std::wstring::npos )
    {
        const auto subText = text.substr(start, end - start);

        AddEntry(subText, true);

        start = end + 1;
    }

    if (start < text.size())
        AddEntry(text.substr(start), false);
}

void Terminal::Print(const Video::TexturePtr& texture)
{
    if (texture)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        GetModel()->AddEntry(std::make_shared<TerminalModel::TextureEntry>(texture));
    }
}

void Terminal::Message(
    const std::wstring& text, const Video::ColorRGBf& color, const Video::TextureFont* font)
{
    Print(text + L'\n', color, font);
}

void Terminal::Error(const std::wstring& text)
{
    Message(L"Error: " + text + L"!", { 1, 0, 0 });
}

void Terminal::Warning(const std::wstring& text)
{
    Message(L"Warning: " + text + L"!", { 1, 1, 0 });
}

void Terminal::Blank()
{
    Print(L"\n");
}

void Terminal::PrintCommand(const std::string& cmd)
{
    Message("> " + cmd, { 0, 1, 0 });
}

void Terminal::Execute(const IO::CommandLine& cmdLine)
{
    if (!cmdLine.IsEmpty())
    {
        /* Execute command with command factory */
        PrintCommand(cmdLine);
        GetCommandFactory()->ExecuteCommand(cmdLine, this);

        /* Store command line in history */
        {
            std::lock_guard<std::mutex> lock(mutex_);
            commandLineHistory.AddCommandLine(cmdLine);
        }
    }
    else
        Blank();

    /* Scroll down to show the entered command */
    GetModel()->scrollBar.ScrollBegin();
}

void Terminal::AppendScript(const std::string& codeLine)
{
    std::lock_guard<std::mutex> lock(mutex_);
    GetModel()->AddEntry(
        std::make_shared<TerminalModel::SourceCodeEntry>(defaultFont_.get(), codeLine)
    );
}

void Terminal::RunScript()
{
    if (scriptEvtHandler_)
        scriptEvtHandler_->OnRunCode(GetModel()->LastScriptCode());
}

void Terminal::UpdateScrollBarSize(const Math::Rectf& viewRect)
{
    GetModel()->scrollBar.SetupSize(GetView()->ViewArea(viewRect).Height());
}

void Terminal::Draw(const Math::Rectf& viewRect, bool showCursor)
{
    std::lock_guard<std::mutex> lock(mutex_);
    UpdateScrollBarSize(viewRect);
    GetView()->Draw(GetRenderer(), viewRect, showCursor);
}

void Terminal::Draw(bool showCursor)
{
    auto renderContext = Video::RenderContext::Active();
    const auto screenSize = renderContext->GetVideoMode().resolution.Cast<float>();
    Draw({ { 0, 0 }, screenSize }, showCursor);
}

void Terminal::Hint()
{
    const auto& cmdLine = GetModel()->GetText();

    if (cmdLine.empty() || GetModel()->mode != TerminalModel::Modes::Default)
        return;

    /* Find suitable command which match the entered sub command name */
    const auto cmdList = GetCommandFactory()->FindSuitableCommands(ToLower(cmdLine));

    if (cmdList.size() > 1)
    {
        /* Change command line to the common divisor (sub command) */
        auto commonDivisor = GetCommandFactory()->FindCommonDivisor(cmdList);
        if (commonDivisor != cmdLine)
        {
            /* Change command line text to common divisor */
            GetModel()->ChangeText(commonDivisor);
        }
        else
        {
            /* Print list of available commands, derived from the entered sub command name */
            Message("--- Several commands found beginning with \"" + cmdLine + "\" ---");
            for (const auto& optCmd : cmdList)
                Message(optCmd.second->Name());
        }

        /* Scroll down to show the hints */
        GetModel()->scrollBar.ScrollBegin();
    }
    else if (cmdList.size() == 1)
    {
        /* Change command line to the found command name */
        const auto& cmd = cmdList.begin()->second;
        if (cmd->Name() != cmdLine)
            GetModel()->ChangeText(cmd->Name());
    }
    else
        Warning("No command found beginning with \"" + cmdLine + "\"");
}

void Terminal::ChangeScrollStride(float stride)
{
    scrollStride_ = std::max(1.0f, stride);
}

void Terminal::ChangeDefaultFont(const Video::TextureFontPtr& defaultFont)
{
    if (defaultFont)
    {
        /*
        Store previous font in the font garbage collector,
        otherwise font shared pointer gets lost.
        */
        fontGarbageCollector_.push_back(defaultFont_);
        defaultFont_ = defaultFont;
    }
}

void Terminal::ErrorNumArgs(const std::string& cmdName)
{
    Error("Invalid number of arguments for command \"" + cmdName + "\" (print \"help\" for more information)");
}


/*
 * KeyboardEventHandler class
 */

Terminal::KeyboardEventHandler::KeyboardEventHandler(Terminal* terminal) :
    terminal_(terminal)
{
}
Terminal::KeyboardEventHandler::~KeyboardEventHandler()
{
}

bool Terminal::KeyboardEventHandler::OnKeyDown(const IO::KeyCodes keyCode, const Platform::Frame* frame)
{
    if (!HasFocus())
        return true;

    auto model = terminal_->GetModel();
    auto view = terminal_->GetView();

    switch (keyCode)
    {
        case IO::KeyCodes::KeyLeft:
            if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
                model->JumpLeft();
            else
                model->MoveCursor(-1);
            view->cursorBlinker.Refresh();
            break;
        
        case IO::KeyCodes::KeyRight:
            if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
                model->JumpRight();
            else
                model->MoveCursor(1);
            view->cursorBlinker.Refresh();
            break;

        case IO::KeyCodes::KeyUp:
            if (terminal_->commandLineHistory.Prev())
                model->ChangeText(terminal_->commandLineHistory.CurrentCommandLine());
            break;
        case IO::KeyCodes::KeyDown:
            if (terminal_->commandLineHistory.Next())
                model->ChangeText(terminal_->commandLineHistory.CurrentCommandLine());
            break;

        case IO::KeyCodes::KeyHome:
            if (!IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
            {
                model->MoveCursorBegin();
                view->cursorBlinker.Refresh();
            }
            else
                model->scrollBar.ScrollEnd();
            break;

        case IO::KeyCodes::KeyPageUp:
            model->scrollBar.ScrollPageForwards();
            break;

        case IO::KeyCodes::KeyPageDown:
            model->scrollBar.ScrollPageBackwards();
            break;

        case IO::KeyCodes::KeyEnd:
            if (!IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
            {
                model->MoveCursorEnd();
                view->cursorBlinker.Refresh();
            }
            else
                model->scrollBar.ScrollBegin();
            break;

        case IO::KeyCodes::KeyDelete:
            model->Remove(
                IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl)
            );
            break;

        case IO::KeyCodes::KeyReturn:
            switch (model->mode)
            {
                case TerminalModel::Modes::Default:
                    terminal_->Execute(model->GetText());
                    break;
                case TerminalModel::Modes::Script:
                    terminal_->AppendScript(model->GetText());
                    break;
            }
            model->ClearText();
            break;

        case IO::KeyCodes::KeyTab:
            terminal_->Hint();
            break;

        case IO::KeyCodes::KeyV:
            if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
            {
                model->Insert(
                    ToStr(Platform::Clipboard::GetClipboardText())
                );
            }
            break;

        case IO::KeyCodes::KeyF1:
            terminal_->Execute("help");
            break;

        case IO::KeyCodes::KeyF5:
            if (model->mode == TerminalModel::Modes::Script)
                terminal_->RunScript();
            break;

        case IO::KeyCodes::KeyEscape:
            if (model->mode == TerminalModel::Modes::Script)
            {
                model->mode = TerminalModel::Modes::Default;
                return false;
            }
            break;

        case IO::KeyCodes::KeyInsert:
            model->insertionEnabled = !model->insertionEnabled;
            view->cursorBlinker.Refresh();
            break;
    }

    return true;
}

void Terminal::KeyboardEventHandler::OnTextEnter(wchar_t chr, const Platform::Frame* frame)
{
    if (!HasFocus())
        return;

    auto model = terminal_->GetModel();
    auto view = terminal_->GetView();

    const char chrUTF8 = static_cast<char>(chr);

    if (chrUTF8 == '\n')
    {
        /* Execute command, when a new line was added */
        terminal_->Execute(model->GetText());
        model->GetText();
    }
    else
    {
        model->Insert(chrUTF8);
        view->cursorBlinker.Refresh();
    }
}


/*
 * MouseEventHandler class
 */

Terminal::MouseEventHandler::MouseEventHandler(Terminal* terminal) :
    terminal_(terminal)
{
}
Terminal::MouseEventHandler::~MouseEventHandler()
{
}

bool Terminal::MouseEventHandler::OnWheelMotion(int motion, const Platform::Frame* frame)
{
    if (HasFocus())
    {
        float scrollMotion = terminal_->GetScrollStride() * motion;

        if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
            scrollMotion *= 5.0f;

        terminal_->GetModel()->scrollBar.Scroll(scrollMotion);
    }
    return true;
}


} // /namespace Utility

} // /namespace Fork



// ========================