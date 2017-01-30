/*
 * Editor application header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_APPLICATION_H__
#define __FORK_EDITOR_APPLICATION_H__


#include "Platform/Core/DynamicLibrary.h"

#include <wx/app.h>


namespace Fork
{

namespace Editor
{


class MainFrame;

//! Main class for the editor application.
class EditorApplication : public wxApp
{
    
    public:
        
        EditorApplication();
        ~EditorApplication();

        /* === Functions === */

        bool LoadLanguageModule(const std::wstring& libraryName);

        void DrawCurrentFrame();

        static EditorApplication* Instance();

        /* === Inline functions === */

        inline MainFrame* GetMainFrame() const
        {
            return mainFrame_;
        }

    private:

        /* === Functions === */

        bool OnInit() override;
        void OnInitCmdLine(wxCmdLineParser& parser) override;
        bool OnCmdLineParsed(wxCmdLineParser& parser) override;
        void OnIdle(wxIdleEvent& event);
        int OnExit() override;

        bool OnExceptionInMainLoop() override;
        void OnUnhandledException() override;

        void UpdateSystem();

        /* === Members === */

        Platform::DynamicLibraryPtr languageLibrary_;

        MainFrame* mainFrame_ = nullptr;

        static EditorApplication* instance_;
        
};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================