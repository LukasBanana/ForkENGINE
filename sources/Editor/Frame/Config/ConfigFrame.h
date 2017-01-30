/*
 * Editor config frame header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_CONFIG_FRAME_H__
#define __FORK_EDITOR_CONFIG_FRAME_H__


#include <wx/propdlg.h>


namespace Fork
{

namespace Editor
{


class ConfigPanelCommon;
class ConfigPanelUI;
class ConfigPanelGraphics;

class ConfigFrame : public wxPropertySheetDialog
{
    
    public:
        
        ConfigFrame(
            wxWindow* parent = nullptr,
            long style = wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxRESIZE_BORDER
        );

        /* === Callback functions === */

        void OnClose(wxCloseEvent& event);

        /* === Functions === */

        bool Show(bool show = true);

    private:
        
        /* === Functions === */

        void BuildFrameLayout();

        /* === Members === */

        ConfigPanelCommon*      panelCommon_    = nullptr;
        ConfigPanelUI*          panelUI_        = nullptr;
        ConfigPanelGraphics*    panelGraphics_  = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================