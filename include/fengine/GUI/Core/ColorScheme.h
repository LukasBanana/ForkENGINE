/*
 * GUI color scheme header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_COLOR_SCHEME_H__
#define __FORK_GUI_COLOR_SCHEME_H__


#include "Core/Export.h"
#include "GUI/Core/BaseGUITypes.h"


namespace Fork
{

namespace GUI
{


//! Common GUI color scheme. This is used for all view controllers.
struct ColorScheme
{
    //! Common box color data (used for frames, buttons, bars etc.).
    struct Box
    {
        Color top;      //!< Title bar first color. By deafult bright color.
        Color bottom;   //!< Title bar second color. By deafult dark color.
        Color title;    //!< Title text color. By deafult gray scaled color.
    };

    //! Common border color data (used for frames, buttons etc.).
    struct Border
    {
        Color outer         = {  30,  30,  30 }; //!< Outer border color. By default black.
        Color innerTop      = { 180, 180, 180 }; //!< Inner-top border color. By default gray.
        Color innerBottom   = {   0,   0,   0 }; //!< Inner-bottom border color. By default black.
    };

    //! Color scheme for frames.
    struct Frame
    {
        Color   border      = {  30,  30,  30 }; //!< Frame border color. By default black.
        Color   ground      = { 230, 230, 230 }; //!< Frame ground color. By default bright gray.
        Box     active      = Box{ { 150, 150, 255 }, { 80, 80, 200 }, { 255, 255, 255 } }; //!< Active frame colors.
        Box     inactive    = Box{ { 100, 100, 255 }, { 50, 50, 170 }, { 255, 255, 255 } }; //!< Inactive frame colors.
        Box     disabled    = Box{ { 170, 170, 200 }, { 80, 80, 130 }, { 170, 170, 170 } }; //!< Disable frame colors.
    };

    //! Color scheme for buttons.
    struct Button
    {
        Border  border; //!< Button border colors.
        Box     normal      = Box{ { 240, 240, 240 }, { 160, 160, 160 }, {  30,  30,  30 } }; //!< Normal button colors.
        Box     selected    = Box{ { 250, 250, 250 }, { 210, 210, 210 }, {  30,  30,  30 } }; //!< Selected button colors.
        Box     pushed      = Box{ { 210, 210, 210 }, { 250, 250, 250 }, {  30,  30,  30 } }; //!< Pushed button colors.
        Box     disabled    = Box{ { 170, 170, 170 }, {  80,  80,  80 }, { 170, 170, 170 } }; //!< Disable button colors.
    };

    //! Color scheme for scroll bars.
    struct ScrollBar
    {
        Color   ground = { 255, 255, 255 }; //!< Scroll bar ground color. By default white.
        Button  buttons; //!< Scroll bar buttons color.
    };

    Frame       frame;
    Button      button;
    ScrollBar   scrollBar;
    Box         statusBar   = Box{ { 210, 210, 210 }, { 170, 170, 170 }, { 0, 0, 0 } }; //!< Status bar colors.
};


//! Sets the new active color scheme. If 'colorScheme' is null, the default color scheme will be set.
FORK_EXPORT void SetupActiveColorScheme(const ColorScheme* colorScheme);

//! Returns a constant reference to the active color scheme.
FORK_EXPORT const ColorScheme& GetActiveColorScheme();


} // /namespace GUI

} // /namespace Fork


#endif



// ========================