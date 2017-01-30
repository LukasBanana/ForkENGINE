/*
 * Editor UI ribbon toolbar header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_RIBBON_TOOL_BAR_H__
#define __FORK_EDITOR_UI_RIBBON_TOOL_BAR_H__


#include <wx/ribbon/toolbar.h>
#include <wx/artprov.h>
#include <wx/bitmap.h>


namespace Fork
{

namespace Editor
{


class UIComboBox;

class UIRibbonToolBar : public wxRibbonToolBar
{
    
    public:
        
        UIRibbonToolBar(wxWindow* parent, wxWindowID id = wxID_ANY);
        virtual ~UIRibbonToolBar();

        static wxBitmap GetBitmap(const wxArtID &Id);

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================