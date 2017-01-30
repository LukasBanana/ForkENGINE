/*
 * WIN32: Platform console manipulation file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Platform/Core/ConsoleManip.h"

#include <Windows.h>
#include <stack>


namespace Fork
{

namespace Platform
{

namespace ConsoleManip
{


// Internal console screen buffer stack.
static std::stack<CONSOLE_SCREEN_BUFFER_INFO> scrBufferInfoStack;

static inline HANDLE StdOut()
{
    return GetStdHandle(STD_OUTPUT_HANDLE);
}

FORK_EXPORT void ChangeColor(const Colors::DataType front)
{
    /* Get current console screen buffer info */
    CONSOLE_SCREEN_BUFFER_INFO bufInfo;
    GetConsoleScreenBufferInfo(StdOut(), &bufInfo);

    /* Setup attributes for new console color */
    WORD attrib = (bufInfo.wAttributes & 0xFFF0);
    
    if ((front & Colors::Red    ) != 0) attrib |= FOREGROUND_RED;
    if ((front & Colors::Green  ) != 0) attrib |= FOREGROUND_GREEN;
    if ((front & Colors::Blue   ) != 0) attrib |= FOREGROUND_BLUE;
    if ((front & Colors::Intens ) != 0) attrib |= FOREGROUND_INTENSITY;
    
    /* Set new console attribute */
    SetConsoleTextAttribute(StdOut(), attrib);
}

FORK_EXPORT void ChangeColor(const Colors::DataType front, const Colors::DataType back)
{
    /* Setup attributes for new console color */
    WORD attrib = 0;

    if ((front & Colors::Red    ) != 0) attrib |= FOREGROUND_RED;
    if ((front & Colors::Green  ) != 0) attrib |= FOREGROUND_GREEN;
    if ((front & Colors::Blue   ) != 0) attrib |= FOREGROUND_BLUE;
    if ((front & Colors::Intens ) != 0) attrib |= FOREGROUND_INTENSITY;

    if ((back & Colors::Red     ) != 0) attrib |= BACKGROUND_RED;
    if ((back & Colors::Green   ) != 0) attrib |= BACKGROUND_GREEN;
    if ((back & Colors::Blue    ) != 0) attrib |= BACKGROUND_BLUE;
    if ((back & Colors::Intens  ) != 0) attrib |= BACKGROUND_INTENSITY;

    /* Set new console attribute */
    SetConsoleTextAttribute(StdOut(), attrib);
}

FORK_EXPORT void QueryColor(Colors::DataType& front, Colors::DataType& back)
{
    /* Get current console screen buffer info */
    CONSOLE_SCREEN_BUFFER_INFO bufInfo;
    GetConsoleScreenBufferInfo(StdOut(), &bufInfo);

    WORD attrib = bufInfo.wAttributes;
    
    /* Extract console colors */
    front = 0;
    back = 0;

    if ((attrib & FOREGROUND_RED        ) != 0) front |= Colors::Red;
    if ((attrib & FOREGROUND_GREEN      ) != 0) front |= Colors::Green;
    if ((attrib & FOREGROUND_BLUE       ) != 0) front |= Colors::Blue;
    if ((attrib & FOREGROUND_INTENSITY  ) != 0) front |= Colors::Intens;

    if ((attrib & BACKGROUND_RED        ) != 0) back |= Colors::Red;
    if ((attrib & BACKGROUND_GREEN      ) != 0) back |= Colors::Green;
    if ((attrib & BACKGROUND_BLUE       ) != 0) back |= Colors::Blue;
    if ((attrib & BACKGROUND_INTENSITY  ) != 0) back |= Colors::Intens;
}

FORK_EXPORT Colors::DataType QueryColor()
{
    Colors::DataType front, back;
    QueryColor(front, back);
    return front;
}

FORK_EXPORT void PushAttrib()
{
    /* Get current console screen buffer info */
    CONSOLE_SCREEN_BUFFER_INFO bufInfo;
    GetConsoleScreenBufferInfo(StdOut(), &bufInfo);

    /* Push buffer info onto stack */
    scrBufferInfoStack.push(bufInfo);
}

FORK_EXPORT void PopAttrib()
{
    if (!scrBufferInfoStack.empty())
    {
        /* Push buffer info onto stack */
        const CONSOLE_SCREEN_BUFFER_INFO bufInfo = scrBufferInfoStack.top();
        scrBufferInfoStack.pop();

        /* Reset previous console screen buffer info */
        SetConsoleTextAttribute(StdOut(), bufInfo.wAttributes);
    }
}

FORK_EXPORT Video::ColorRGBf ToColor(const Colors::DataType colorFlags)
{
    Video::ColorRGBf color
    {
        ( colorFlags & Colors::Red   ) != 0 ? 1.0f : 0.0f,
        ( colorFlags & Colors::Green ) != 0 ? 1.0f : 0.0f,
        ( colorFlags & Colors::Blue  ) != 0 ? 1.0f : 0.0f
    };

    if ((colorFlags & Colors::Intens) == 0)
        color *= 0.5f;

    return color;
}

FORK_EXPORT std::string ToHexColor(const Colors::DataType colorFlags)
{
    std::string hexColor;

    if ((colorFlags & Colors::Intens) != 0)
    {
        hexColor += (( colorFlags & Colors::Red   ) != 0 ? "FF" : "00");
        hexColor += (( colorFlags & Colors::Green ) != 0 ? "FF" : "00");
        hexColor += (( colorFlags & Colors::Blue  ) != 0 ? "FF" : "00");
    }
    else
    {
        hexColor += (( colorFlags & Colors::Red   ) != 0 ? "80" : "00");
        hexColor += (( colorFlags & Colors::Green ) != 0 ? "80" : "00");
        hexColor += (( colorFlags & Colors::Blue  ) != 0 ? "80" : "00");
    }

    return hexColor;
}


} // /namespace ConsoleManip

} // /namespace Platform

} // /namespace Fork



// ========================