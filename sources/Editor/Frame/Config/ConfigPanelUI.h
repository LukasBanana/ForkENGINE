/*
 * Editor config panel UI header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_CONFIG_PANEL_UI_H__
#define __FORK_EDITOR_CONFIG_PANEL_UI_H__


#include <wx/panel.h>
#include <wx/propgrid/propgrid.h>


namespace Fork
{

namespace Editor
{


class UIPropertyGrid;

class ConfigPanelUI : public wxPanel
{
    
    public:
        
        ConfigPanelUI(wxWindow* parent = nullptr);

        /* === Callback functions === */

        void OnApplySettings(wxPropertyGridEvent& event);

    private:
        
        /* === Functions === */

        void BuildPanelLayout();

        /* === Members === */

        UIPropertyGrid* propGrid_ = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================