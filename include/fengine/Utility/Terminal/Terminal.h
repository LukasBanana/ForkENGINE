/*
 * Terminal header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_TERMINAL_H__
#define __FORK_UTILITY_TERMINAL_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Utility/Terminal/TerminalModel.h"
#include "Utility/Terminal/TerminalView.h"
#include "Utility/Terminal/CommandFactory.h"
#include "Video/Font/TextureFont.h"
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"
#include "IO/Core/CommandLineHistory.h"
#include "IO/InputDevice/Keyboard.h"
#include "IO/InputDevice/Mouse.h"
#include "IO/Core/Log.h"

#include <mutex>
#include <stack>


namespace Fork
{

namespace Utility
{


DECL_SHR_PTR(Terminal);

/**
Terminal controller class. The terminal can be used as in-game command line.
It is designed with the model-view-controller pattern, where "TerminalModel"
contains the model data, and "TerminalView" provides functions to draw the terminal.
Both TerminalModel and TerminalView can be extended.
\remarks As long as you use the terminal only over this interface,
and not over the model (GetModel) and the view (GetView), you can use it in multi-threading.
The terminal has one common mutex as lock guard, when drawing the view, printing
text or executing command lines. Once you access the model or the view,
don't use it in a multi-threading context, or make your own lock guards!
\see TerminalModel
\see TerminalView
\see GetModel
\see GetView
\see Draw
*/
class FORK_EXPORT Terminal
{

    public:
        
        //! Terminal script event handler interface.
        class FORK_EXPORT ScriptEventHandler
        {
            
            public:
                
                virtual ~ScriptEventHandler()
                {
                }

                /**
                This is the "run script" event callback. This is occurs when "Terminal::RunScript" is called.
                \param[in] sourceCode Specifies the source code which is to be executed.
                \see Terminal::RunScript
                */
                virtual void OnRunCode(const std::string& sourceCode)
                {
                }

        };

        typedef std::shared_ptr<ScriptEventHandler> ScriptEventHandlerPtr;

        //! Terminal log event handler implementation.
        class FORK_EXPORT LogEventHandler : public IO::Log::EventHandler
        {

            public:
                
                //! \throws NullPointerException If 'terminal' is null.
                LogEventHandler(Terminal* terminal);
                
                void OnPrintLn(const std::string& indent, const std::string& message, const IO::Log::EntryTypes type) override;
                void OnPrint(const std::string& message, const IO::Log::EntryTypes type) override;
                
                void OnStartLn(const std::string& indent) override;
                void OnEndLn() override;

                void OnPushColor(const IO::Log::ColorFlags& frontColorFlags) override;
                void OnPushColor(const IO::Log::ColorFlags& frontColorFlags, const IO::Log::ColorFlags& backColorFlags) override;
                void OnPopColor() override;

                void OnBlank() override;

            protected:
                
                /**
                Returns the active front color for terminal messages.
                This is the converted color from the first element of the color stack.
                \see colorStack
                */
                Video::ColorRGBf ActiveColor() const;

                inline Terminal* GetTerminal() const
                {
                    return terminal_;
                }

                //! Stack for the 'front' color flags. The 'back' color flags will be ignored for the terminal.
                std::stack<IO::Log::ColorFlags> colorStack;

            private:
                
                Terminal* terminal_ = nullptr;

        };

        /* === Functions === */

        /**
        Terminal constructor.
        \param[in] renderer Shared pointer to a primitive renderer.
        \param[in] commandFactory Optional shared pointer to a terminal command factory. If this is null, the default command factory will be created.
        \param[in] model Optional shared pointer to a terminal model. If this is null, the default terminal model will be created.
        \param[in] view Optional shared pointer to a terminal view. If this is null, the default terminal view will be created.
        \param[in] defaultFont Optional shared pointer to the a defualt terminal font. If this is null, the default font ("courier new") will be created.
        \throws NullPointerException If 'renderer' is null.
        \see Video::PrimitiveRenderer
        \see ScriptEventHandler
        */
        Terminal(
            const Video::PrimitiveRendererPtr& renderer,
            const CommandFactoryPtr& commandFactory = nullptr,
            const TerminalModelPtr& model = nullptr,
            const TerminalViewPtr& view = nullptr,
            const Video::TextureFontPtr& defaultFont = nullptr
        );
        virtual ~Terminal();

        /**
        Enables or disables the terminal.
        \param[in] isEnabled Specifies whether the terminal is to be enabled or disbaled.
        This will also lock (if true) or unlock (if false) the keyboard and mouse focus. By default flase.
        \return True if the terminal could be enabled or disabled. Otherwise the keyboard and/or
        mouse focus was already locked other event handlers.
        */
        bool Enable(bool isEnabled);

        /**
        Adds new text entries to the terminal.
        \param[in] text Specifies the text which is to be printed.
        To make a new line, this text must contain the new-line character '\\n'.
        \param[in] color Specifies the color for this text entry.
        \param[in] font Constant raw-pointer to the font, which is to be used for drawing the text.
        If this is null, the default font will be used (which is "courier new" with size 15). By default null.
        \note The font is not a shared pointer, because it will be frequently referenced.
        So make sure you have a shared pointer which exists as long as this terminal.
        \note Make sure the characters are supported for the font you use!
        */
        virtual void Print(
            const std::wstring& text, const Video::ColorRGBf& color = {},
            const Video::TextureFont* font = nullptr
        );
        /**
        Adds a new texture entry to the terminal.
        \param[in] texture Shared pointer to the texture object.
        */
        virtual void Print(const Video::TexturePtr& texture);

        //! Prints a simple message. This does not require the new-line character '\\n'.
        virtual void Message(
            const std::wstring& text, const Video::ColorRGBf& color = {}, const Video::TextureFont* font = nullptr
        );
        //! Prints a simple error message. This does not require the new-line character '\\n'.
        virtual void Error(const std::wstring& text);
        //! Prints a simple warning message. This does not require the new-line character '\\n'.
        virtual void Warning(const std::wstring& text);
        //! Prints a blank line.
        virtual void Blank();

        /**
        Prints the specified command. This is equivalent to the following code:
        \code
        terminal->Message("> " + cmd, { 0, 1, 0 });
        \endcode
        */
        virtual void PrintCommand(const std::string& cmd);

        /**
        Prints and executes the specified command.
        \see Utility::TerminalCommandExecutor
        \see PrintCommand
        */
        virtual void Execute(const IO::CommandLine& cmdLine);

        //! Appends a new line of code if the script mode is active.
        virtual void AppendScript(const std::string& codeLine);

        /**
        Runs the current entered script.
        \see Lang::FSCInterpreter
        */
        virtual void RunScript();

        /**
        Draws the terminal. This is equivalent to the following code:
        \code
        terminal->GetView()->Draw(terminal->GetRenderer(), viewRect, showCursor);
        \endcode
        \see TerminalView::Draw
        */
        virtual void Draw(const Math::Rectf& viewRect, bool showCursor = true);

        /**
        Draws the terminal over the entire screen.
        \see Draw(const Math::Rectf&)
        \see TerminalView::Draw
        */
        virtual void Draw(bool showCursor = true);

        /**
        Extends the command line with a suitable command.
        If several commands fit the entered command line, the list will be printed,
        e.g. "cle" will be extended to "clear".
        */
        virtual void Hint();

        /**
        Sets the scroll stride (in pixels).
        \param[in] stride Specifies the new scroll stride.
        This will be clamped to [1.0 .. +inf). By default 45.0.
        \see GetScrollStride
        */
        void ChangeScrollStride(float stride);

        /**
        Changes the default font for this terminal.
        \param[in] defaultFont Shared pointer to the new default font.
        If this is null, the function call has no effect.
        */
        void ChangeDefaultFont(const Video::TextureFontPtr& defaultFont);

        /**
        Updates the terminal model's scroll bar size. This will be called automatically when 'Draw' is called.
        \param[in] viewRect Specifies the entire terminal view rectangle. This should be the same as for the 'Draw' function.
        \see Draw
        */
        void UpdateScrollBarSize(const Math::Rectf& viewRect);

        //! Prints an information that the number of arguments are wrong for the specified command.
        void ErrorNumArgs(const std::string& cmdName);

        /* === Inline functions === */

        //! Returns true if this terminal is enabled.
        inline bool IsEnabled() const
        {
            return isEnabled_;
        }

        //! Returns a raw-pointer to the terminal model.
        inline TerminalModel* GetModel()
        {
            return model_.get();
        }
        //! Returns a constant raw-pointer to the terminal model.
        inline const TerminalModel* GetModel() const
        {
            return model_.get();
        }

        //! Returns a raw-pointer to the command factory.
        inline CommandFactory* GetCommandFactory() const
        {
            return commandFactory_.get();
        }
        //! Returns a raw-pointer to the terminal view.
        inline TerminalView* GetView()
        {
            return view_.get();
        }
        //! Returns a constant raw-pointer to the terminal view.
        inline const TerminalView* GetView() const
        {
            return view_.get();
        }

        //! Returns the primitive renderer which is used to draw the terminal.
        inline Video::PrimitiveRenderer* GetRenderer() const
        {
            return renderer_.get();
        }

        //! Returns a constant raw-pointer to the default font.
        inline const Video::TextureFont* GetDefaultFont() const
        {
            return defaultFont_.get();
        }

        /**
        Sets the script event handler. This there is no script event handler,
        no scripting will be performed by this terminal.
        \see ScriptEventHandler
        */
        inline void SetScriptEventHandler(const ScriptEventHandlerPtr& scriptEvtHandler)
        {
            scriptEvtHandler_ = scriptEvtHandler;
        }

        /**
        Returns the scroll stride (in pixels).
        \see TerminalModel::Scroll
        */
        inline float GetScrollStride() const
        {
            return scrollStride_;
        }

        //! \see Print(const std::wstring&, const Video::ColorRGBf&, const Video::TextureFont*)
        inline void Print(
            const std::string& text, const Video::ColorRGBf& color = {},
            const Video::TextureFont* font = nullptr)
        {
            Print(UTF8toUTF16(text), color, font);
        }
        //! \see Message(const std::wstring&, const Video::ColorRGBf&, const Video::TextureFont*)
        inline void Message(
            const std::string& text, const Video::ColorRGBf& color = {}, const Video::TextureFont* font = nullptr)
        {
            Message(UTF8toUTF16(text), color, font);
        }
        //! \see Error(const std::wstring&)
        inline void Error(const std::string& text)
        {
            Error(UTF8toUTF16(text));
        }
        //! \see Warning(const std::wstring&)
        inline void Warning(const std::string& text)
        {
            Warning(UTF8toUTF16(text));
        }

        /* === Members === */

        //! Command line history.
        IO::CommandLineHistory commandLineHistory;

    protected:

        /* === Classes === */

        class FORK_EXPORT KeyboardEventHandler : public IO::Keyboard::EventHandler
        {
            
            public:
                
                KeyboardEventHandler(Terminal* terminal);
                virtual ~KeyboardEventHandler();

                virtual bool OnKeyDown(const IO::KeyCodes keyCode, const Platform::Frame* frame) override;
                virtual void OnTextEnter(wchar_t chr, const Platform::Frame* frame) override;

            private:
                
                Terminal* terminal_ = nullptr;

        };

        class FORK_EXPORT MouseEventHandler : public IO::Mouse::EventHandler
        {
            
            public:
                
                MouseEventHandler(Terminal* terminal);
                virtual ~MouseEventHandler();

                virtual bool OnWheelMotion(int motion, const Platform::Frame* frame) override;

            private:
                
                Terminal* terminal_ = nullptr;

        };

    private:

        /* === Members === */

        Video::PrimitiveRendererPtr             renderer_;

        CommandFactoryPtr                       commandFactory_;
        TerminalModelPtr                        model_;
        TerminalViewPtr                         view_;

        Video::TextureFontPtr                   defaultFont_;
        std::vector<Video::TextureFontPtr>      fontGarbageCollector_;

        ScriptEventHandlerPtr                   scriptEvtHandler_;

        std::shared_ptr<KeyboardEventHandler>   keyboardEventHandler_;
        std::shared_ptr<MouseEventHandler>      mouseEventHandler_;

        float                                   scrollStride_           = 45.0f;
        bool                                    isEnabled_              = false;

        mutable std::mutex                      mutex_;                             //!< General terminal mutex.

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================