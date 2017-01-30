/*
 * Editor UI combobox file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIComboBox.h"


namespace Fork
{

namespace Editor
{


UIComboBox::UIComboBox(
    wxWindow* parent, const wxPoint& position, const wxSize& size, const wxString& value, long style) :
        wxComboBox(parent, wxID_ANY, value, position, size, 0, 0, style)
{
}
UIComboBox::~UIComboBox()
{
}

wxString UIComboBox::GetSelectedString() const
{
    return GetString(GetSelection());
}

void UIComboBox::AddItems(const std::initializer_list<wxString>& items)
{
    for (const auto& item : items)
        Append(item);
}


} // /namespace Editor

} // /namespace Fork



// ========================