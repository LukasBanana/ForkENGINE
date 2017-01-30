/*
 * Editor UI property float file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIPropertyFloat.h"
#include "IO/FileSystem/File.h"
#include "../../UIExtensions/UIPropertyGrid.h"


namespace Fork
{

namespace Editor
{


UIPropertyFloat::UIPropertyFloat(UIPropertyGrid& propGrid, const wxString& propName) :
    prop_{ propGrid.FindBoundedProperty<wxFloatProperty>(propName) }
{
}

void UIPropertyFloat::WriteToProperty()
{
    prop_->SetValue(wxVariant(value));
}

void UIPropertyFloat::ReadFromProperty(const PropertyState& state)
{
    value = state.GetFloat();
}


} // /namespace Editor

} // /namespace Fork



// ========================