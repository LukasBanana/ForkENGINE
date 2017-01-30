/*
 * Editor static leyout configuration header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_STATIC_LAYOUT_CONFIG_H__
#define __FORK_EDITOR_STATIC_LAYOUT_CONFIG_H__


#include "Math/Core/Size2.h"

#include <wx/gdicmn.h>


namespace Fork
{

namespace Editor
{

namespace StaticLayout
{


namespace MainFrameLayout
{
    namespace StatusBarLayout
    {
        static const Math::Size2i progressBarSize     { 400, 15 };
        static const Math::Size2i errorHintButtonSize { 18, 18 };
    }

    static const wxSize defaultSize { 900, 600 };
    static const wxSize minSize     { 400, 400 };
}

namespace ConfigFrameLayout
{
    static const int    buttonBorder    { 5 };
    static const wxSize defaultSize     { 500, 400 };
    static const wxSize minSize         { 300, 300 };
}

namespace ConsoleFrameLayout
{
    static const wxSize defaultSize { 500, 400 };
    static const wxSize minSize     { 200, 200 };
}

namespace HierarchyViewerFrameLayout
{
    static const wxSize defaultSize { 400, 600 };
    static const wxSize minSize     { 200, 200 };
}

namespace EditorViewLayout
{
    static const float profilerTextBorder = 15.0f;
}


} // /namespace StaticLayout

} // /namespace Editor

} // /namespace Fork


#endif



// ========================