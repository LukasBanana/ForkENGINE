/*
 * Editor configuration header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_CONFIG_H__
#define __FORK_EDITOR_CONFIG_H__


#include "ColorScheme.h"
#include "Video/Monitor/ContextDescription.h"


namespace Fork
{

namespace Editor
{


//! Class for external editor configurations.
class Config
{
    
    public:
        
        Config(const Config&) = delete;
        Config& operator = (const Config&) = delete;

        //! Default settings filename. This is "Settings.xml".
        static const std::string settingsFilename;
        //! Default layout filename. This is "Layout".
        static const std::string layoutFilename;
        
        /* === Functions === */

        //! Loads all external configurations (e.g. color schemes).
        void LoadConfiguration();

        //! \todo Not yet implemented!
        bool LoadSettings(const std::string& filename);
        //! \todo Not yet implemented!
        bool SaveSettings(const std::string& filename);

        bool LoadSettings();
        bool SaveSettings();

        bool LoadLayout(const std::string& filename);
        bool SaveLayout(const std::string& filename);

        bool LoadLayout();
        bool SaveLayout();

        static Config* Instance();

        /* === Members === */

        //! Color scheme list.
        std::vector<ColorScheme> colorSchemes;

        //! Render system library module. By default "ForkRendererGL" (OpenGL).
        std::wstring renderSystemModule = L"ForkRendererGL";

        //! Physics sytsem library module. By default "ForkPhysicsNw" (Newton Game Dyanmics).
        std::wstring physicsSystemModule = L"ForkPhysicsNw";

        //! Script interpreter library module. By default "ForkScriptPy" (Python).
        std::wstring scriptInterpreterModule = L"ForkScriptPy";

        //! Language library module. By default "ForkEdLanguageEN" (English).
        std::wstring languageModule = L"ForkEdLanguageEN";

        //! Render context description.
        Video::ContextDescription renderContextDesc { true, true, 8 };

    private:
        
        Config() = default;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================