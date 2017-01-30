/*
 * Editor UI property string file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIPropertyString.h"
#include "IO/FileSystem/File.h"
#include "../../UIExtensions/UIPropertyGrid.h"


namespace Fork
{

namespace Editor
{


UIPropertyString::UIPropertyString(UIPropertyGrid& propGrid, const wxString& propName) :
    prop_{ propGrid.FindBoundedProperty<wxStringProperty>(propName) }
{
}

void UIPropertyString::WriteToProperty()
{
    prop_->SetValue(wxVariant(wxString(value)));
}

void UIPropertyString::ReadFromProperty(const PropertyState& state)
{
    value = state.GetString();
}


} // /namespace Editor

} // /namespace Fork



// ========================