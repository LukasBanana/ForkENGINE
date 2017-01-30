/*
 * Main frame tool bar composition header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_MAIN_FRAME_TOOL_BAR_COMPOSITION_H__
#define __FORK_EDITOR_MAIN_FRAME_TOOL_BAR_COMPOSITION_H__


#include <wx/window.h>

#include <array>


namespace Fork
{

namespace Editor
{


class UIToolBar;

class MainFrameToolBarComposition
{
    
    public:
        
        struct ToolBars
        {
            enum : size_t
            {
                Main = 0,
                Transform,
                View,

                __Num__,
            };
        };

        MainFrameToolBarComposition(wxWindow* parent);

        void Show(bool show = true);

        inline const std::array<UIToolBar*, ToolBars::__Num__>& GetToolBars() const
        {
            return toolBars_;
        }

    private:

        /* === Functions === */

        void BuildToolBarMain       (UIToolBar* toolBar);
        void BuildToolBarTransform  (UIToolBar* toolBar);
        void BuildToolBarView       (UIToolBar* toolBar);

        /* === Members === */

        std::array<UIToolBar*, ToolBars::__Num__> toolBars_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================