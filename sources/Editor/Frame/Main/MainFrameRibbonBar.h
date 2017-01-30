/*
 * Main frame ribbon bar header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_MAIN_FRAME_RIBBON_BAR_H__
#define __FORK_EDITOR_MAIN_FRAME_RIBBON_BAR_H__


#include <wx/ribbon/bar.h>


namespace Fork
{

namespace Editor
{


class MainFrameRibbonBar : public wxRibbonBar
{
    
    public:
        
        struct Pages
        {
            enum : size_t
            {
                Main = 0,

                __Num__,
            };
        };

        MainFrameRibbonBar(wxWindow* parent);

    private:

        /* === Functions === */

        void BuildPageMain      (wxRibbonPage* page);
        void BuildPageTerrain   (wxRibbonPage* page);
        void BuildPageGame      (wxRibbonPage* page);

        void BuildPageMainPanelToolbar          (wxRibbonPanel* panel);
        void BuildPageMainPanelTransformation   (wxRibbonPanel* panel);
        void BuildPageMainPanelView             (wxRibbonPanel* panel);

        /* === Members === */



};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================