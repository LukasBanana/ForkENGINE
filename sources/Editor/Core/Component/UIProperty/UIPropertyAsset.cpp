/*
 * Editor UI property asset file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIPropertyAsset.h"
#include "IO/FileSystem/File.h"
#include "../../UIExtensions/UIPropertyGrid.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace Editor
{


UIPropertyAsset::UIPropertyAsset(UIPropertyGrid& propGrid, const wxString& propName) :
    prop_{ propGrid.FindBoundedProperty<wxStringProperty>(propName) }
{
}

void UIPropertyAsset::WriteToProperty()
{
    prop_->SetValue(wxVariant(wxString(ToStr(value))));
}

void UIPropertyAsset::ReadFromProperty(const PropertyState& state)
{
    value = StrToNum<unsigned int>(state.GetString());
}


} // /namespace Editor

} // /namespace Fork



// ========================