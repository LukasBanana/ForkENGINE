/*
 * Editor console frame header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_CONSOLE_FRAME_H__
#define __FORK_EDITOR_CONSOLE_FRAME_H__


#include "../EditorFrame.h"
#include "Utility/Terminal/Terminal.h"
#include "IO/Core/Log.h"
#include "IO/FileSystem/LogFile.h"
#include "Script/ScriptInterpreter.h"

#include <wx/statusbr.h>
#include <stack>


namespace Fork
{

namespace Editor
{


class UIScreenFrame;

class ConsoleFrame : public EditorFrame
{
    
    public:
        
        ConsoleFrame(wxWindow* parent = nullptr);
        ~ConsoleFrame();

        /* === Callbacks === */

        void OnFocus    (wxFocusEvent&  event);
        void OnLostFocus(wxFocusEvent&  event);
        void OnMove     (wxMoveEvent&   event);

        /* === Functions === */

        void Draw();

        /**
        Starts the pre-log event handler.
        \remarks This is used to allow printing messages to the log output
        before the terminal has been created. When the console frame is created,
        this pre-log event handler will be flushed to show all previous messages
        in the terminal.
        */
        static void StartPreLogEventHandler();
        //! Opens the log file.
        static bool OpenLogFile(const std::string& filename);

        /**
        Returns true if the specified message refers to an error message.
        This is the case if the message starts with "Error" (case insensitive).
        */
        static bool IsErrorMessage(const std::string& message);

        /* === Inline functions === */

        inline Utility::Terminal* GetTerminal() const
        {
            return terminal_.get();
        }

    private:
        
        /* === Classes === */

        class BaseLogEventHandler : public IO::Log::EventHandler
        {
            
            public:
                
                void OnPushColor(const ColorFlags& frontColorFlags) override;
                void OnPushColor(const ColorFlags& frontColorFlags, const ColorFlags& backColorFlags) override;
                void OnPopColor() override;

            protected:
                
                inline const Video::ColorRGBf& GetCurrentColor() const
                {
                    return currentColor_;
                }

            private:
                
                std::stack<Video::ColorRGBf> colorStack_;
                Video::ColorRGBf currentColor_;

        };

        class PreLogEventHandler : public BaseLogEventHandler
        {
            
            public:
                
                void OnPrintLn(const std::string& indent, const std::string& message, const IO::Log::EntryTypes type) override;

                //! Flushes the entire pre-log buffer.
                void Flush(Utility::Terminal& terminal);

            private:
                
                struct Message
                {
                    std::string         text;
                    Video::ColorRGBf    color;
                };

                std::vector<Message> messages_;

        };

        class LogEventHandler : public BaseLogEventHandler
        {
            
            public:
                
                LogEventHandler(Utility::Terminal* terminal);

                void OnPrint(const std::string& message, const IO::Log::EntryTypes type) override;

                void OnStartLn(const std::string& indent) override;
                void OnEndLn() override;

            private:
                
                Utility::Terminal* terminal_ = nullptr;

        };

        class LogFileEventHandler : public IO::LogFile::LogHTMLEventHandler
        {
        
            public:
            
                LogFileEventHandler(const std::string& filename);

        };

        class TerminalScriptEventHandler : public Utility::Terminal::ScriptEventHandler
        {
            
            public:
                
                TerminalScriptEventHandler(Script::ScriptInterpreter* interpreter);

                void OnRunCode(const std::string& code) override;

            private:
                
                Script::ScriptInterpreter* interpreter_ = nullptr;

        };

        /* === Functions === */

        bool BuildFrameLayout();
        void CreateTerminal();
        void CreateEventHandler();

        void UpdateStatusText();

        /* === Members === */

        UIScreenFrame*                                  screenFrame_    = nullptr;

        wxStatusBar*                                    statusBar_      = nullptr;

        std::unique_ptr<Utility::Terminal>              terminal_;
        std::shared_ptr<LogEventHandler>                logEventHandler_;

        static std::shared_ptr< PreLogEventHandler  >   preLogEventHandler_;
        static std::shared_ptr< LogFileEventHandler >   logFileEventHandler_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================