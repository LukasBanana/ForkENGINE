/*
 * Platform console manipulation header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PLATFORM_CONSOLE_MANIP_H__
#define __FORK_PLATFORM_CONSOLE_MANIP_H__


#include "Core/Export.h"
#include "Video/Core/ColorRGB.h"

#include <string>


namespace Fork
{

namespace Platform
{

namespace ConsoleManip
{


/* === Enumerations === */

struct Colors
{
    typedef unsigned int DataType;
    enum : DataType
    {
        Red     = (1 << 0),
        Green   = (1 << 1),
        Blue    = (1 << 2),
        
        Intens  = (1 << 3),

        Black   = 0,
        Gray    = (Red | Green | Blue),
        White   = (Gray | Intens),

        Yellow  = (Red | Green),
        Pink    = (Red | Blue),
        Cyan    = (Green | Blue)
    };
};


/* === Functions === */

//! Sets the console front color.
FORK_EXPORT void ChangeColor(const Colors::DataType front);
//! Sets the console front- and back colors.
FORK_EXPORT void ChangeColor(const Colors::DataType front, const Colors::DataType back);

//! Retreives the current console front- and back colors.
FORK_EXPORT void QueryColor(Colors::DataType& front, Colors::DataType& back);
//! Returns the current console front color.
FORK_EXPORT Colors::DataType QueryColor();

//! Pushes the current console color attributes onto the internal stack.
FORK_EXPORT void PushAttrib();
//! Pops the previous console color attributes from the internal stack.
FORK_EXPORT void PopAttrib();

//! Converts the color flags into a real color.
FORK_EXPORT Video::ColorRGBf ToColor(const Colors::DataType colorFlags);
//! Converts the color flags into a hex color (as it is used in HTML documents). The format is "RRGGBB".
FORK_EXPORT std::string ToHexColor(const Colors::DataType colorFlags);


/* === Structures === */

/**
Scoped color structure.
\code
{
    // Only the constructor and destructor this structure will be used
    Platform::ConsoleManip::ScopedColor color { Platform::ConsoleManip::Colors::Yellow };

    // Print a message with the current scoped color
    IO::Consoel::PrintNL("Hello, World!");
}

// Print a message with the previous color
IO::Console::PrintNL("Hello, World!");
\endcode
*/
struct ScopedColor
{
    inline ScopedColor(const Colors::DataType& front)
    {
        PushAttrib();
        ChangeColor(front);
    }
    inline ScopedColor(const Colors::DataType& front, const Colors::DataType& back)
    {
        PushAttrib();
        ChangeColor(front, back);
    }
    inline ~ScopedColor()
    {
        PopAttrib();
    }
};


} // /namespace ConsoleManip

} // /namespace Platform

} // /namespace Fork


#endif



// ========================