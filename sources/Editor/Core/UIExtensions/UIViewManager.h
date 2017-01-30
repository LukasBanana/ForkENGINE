/*
 * Editor UI view manager header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_VIEW_MANAGER_H__
#define __FORK_EDITOR_UI_VIEW_MANAGER_H__


#include "../ColorScheme.h"

#include <wx/aui/aui.h>
#include <wx/aui/auibar.h>
#include <wx/aui/framemanager.h>
#include <wx/artprov.h>


namespace Fork
{

namespace Editor
{


class UIViewManager : public wxAuiManager
{
    
    public:
        
        UIViewManager(
            wxWindow* managedWindow, long flags = wxAUI_MGR_DEFAULT | wxAUI_MGR_LIVE_RESIZE
        );
        virtual ~UIViewManager();

        int ArtIconSize() const;

        void ChangeColorScheme(const ColorScheme& scheme);

        static void ChangeColorSchemeForAll(const ColorScheme& scheme);

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================