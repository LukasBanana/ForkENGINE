/*
 * Editor UI button file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIButton.h"


namespace Fork
{

namespace Editor
{


UIButton::UIButton(
    wxWindow* parent, const wxPoint& position, const wxSize& size, const wxString& title, long style) :
        wxButton(parent, wxID_ANY, title, position, size, style)
{
}
UIButton::~UIButton()
{
}


} // /namespace Editor

} // /namespace Fork



// ========================