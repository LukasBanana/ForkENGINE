/*
 * Editor UI property enum file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIPropertyEnum.h"
#include "IO/FileSystem/File.h"
#include "../../UIExtensions/UIPropertyGrid.h"


namespace Fork
{

namespace Editor
{


UIPropertyEnum::UIPropertyEnum(UIPropertyGrid& propGrid, const wxString& propName) :
    prop_{ propGrid.FindBoundedProperty<wxEnumProperty>(propName) }
{
}

void UIPropertyEnum::WriteToProperty()
{
    prop_->SetValue(wxVariant(value));
}

void UIPropertyEnum::ReadFromProperty(const PropertyState& state)
{
    value = state.GetInt();
}


} // /namespace Editor

} // /namespace Fork



// ========================