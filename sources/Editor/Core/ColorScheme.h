/*
 * Editor color scheme header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_COLOR_SCHEME_H__
#define __FORK_EDITOR_COLOR_SCHEME_H__


#include <wx/colour.h>
#include <string>
#include <vector>


namespace Fork
{

namespace Editor
{


//! Color scheme structure.
struct ColorScheme
{
    wxString name;

    /* --- UIViewManager colors --- */
    wxColour lineColor;
    wxColour captionColor;
    wxColour gradientColor;
    wxColour sashColor;

    /* --- UIPropertyGrid colors --- */
    wxColour cellBgColor;
    wxColour captionBgColor;
    wxColour disabledTextColor;
    wxColour selectionBgColor;
    wxColour selectionTextColor;
    wxColour captionTextColor;
    wxColour cellTextColor;

    inline wxColour* ColorPtr()
    {
        return &lineColor;
    }
};


//! Loads colors schemes from the specified XML file.
std::vector<ColorScheme> LoadColorSchemes(const std::string& filename);

void ChangeColorScheme(const ColorScheme& scheme);


} // /namespace Editor

} // /namespace Fork


#endif



// ========================