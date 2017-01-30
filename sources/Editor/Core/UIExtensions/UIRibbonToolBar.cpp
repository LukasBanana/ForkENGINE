/*
 * Editor UI ribbon toolbar file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIRibbonToolBar.h"
#include "../WxHelper.h"


namespace Fork
{

namespace Editor
{


UIRibbonToolBar::UIRibbonToolBar(wxWindow* parent, wxWindowID id) :
    wxRibbonToolBar(parent, id)
{
}
UIRibbonToolBar::~UIRibbonToolBar()
{
}

wxBitmap UIRibbonToolBar::GetBitmap(const wxArtID &Id)
{
    return wxArtProvider::GetBitmap(Id, wxART_MENU, wxSize(16, 16));
}


} // /namespace Editor

} // /namespace Fork



// ========================