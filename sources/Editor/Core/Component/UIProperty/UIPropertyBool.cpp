/*
 * Editor UI property boolean file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIPropertyBool.h"
#include "IO/FileSystem/File.h"
#include "../../UIExtensions/UIPropertyGrid.h"


namespace Fork
{

namespace Editor
{


UIPropertyBool::UIPropertyBool(UIPropertyGrid& propGrid, const wxString& propName) :
    prop_{ propGrid.FindBoundedProperty<wxBoolProperty>(propName) }
{
}

void UIPropertyBool::WriteToProperty()
{
    prop_->SetValue(wxVariant(value));
}

void UIPropertyBool::ReadFromProperty(const PropertyState& state)
{
    value = state.GetBool();
}


} // /namespace Editor

} // /namespace Fork



// ========================