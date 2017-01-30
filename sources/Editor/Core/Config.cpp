/*
 * Editor configuration file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Config.h"
#include "../Frame/Main/MainFrame.h"
#include "IO/FileSystem/PhysicalFile.h"
#include "Paths.h"


namespace Fork
{

namespace Editor
{


const std::string Config::settingsFilename  = "Settings.xml";
const std::string Config::layoutFilename    = "Layout";

void Config::LoadConfiguration()
{
    colorSchemes = LoadColorSchemes(Paths::dirConfig + "ColorSchemes.xml");
}

bool Config::LoadSettings(const std::string& filename)
{
    //...
    return true;
}

bool Config::SaveSettings(const std::string& filename)
{
    //...
    return true;
}

bool Config::LoadSettings()
{
    return LoadSettings(Paths::dirConfig + Config::settingsFilename);
}

bool Config::SaveSettings()
{
    return SaveSettings(Paths::dirConfig + Config::settingsFilename);
}

bool Config::LoadLayout(const std::string& filename)
{
    /* Create session file */
    IO::PhysicalFile file;
    if (!file.Open(filename, IO::File::OpenFlags::Read))
        return false;

    /* Load frame (AUI) sessions */
    MainFrame::Instance()->LoadLayout(file);

    return true;
}

bool Config::SaveLayout(const std::string& filename)
{
    /* Create session file */
    IO::PhysicalFile file;
    if (!file.Open(filename, IO::File::OpenFlags::Write))
        return false;

    /* Save frame (AUI) sessions */
    MainFrame::Instance()->SaveLayout(file);

    return true;
}

bool Config::LoadLayout()
{
    return LoadLayout(Paths::dirConfig + Config::layoutFilename);
}

bool Config::SaveLayout()
{
    return SaveLayout(Paths::dirConfig + Config::layoutFilename);
}

Config* Config::Instance()
{
    static Config instance;
    return &instance;
}


} // /namespace Editor

} // /namespace Fork



// ========================