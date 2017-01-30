/*
 * Editor UI property integer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIPropertyInt.h"
#include "IO/FileSystem/File.h"
#include "../../UIExtensions/UIPropertyGrid.h"


namespace Fork
{

namespace Editor
{


UIPropertyInt::UIPropertyInt(UIPropertyGrid& propGrid, const wxString& propName) :
    prop_{ propGrid.FindBoundedProperty<wxIntProperty>(propName) }
{
}

void UIPropertyInt::WriteToProperty()
{
    prop_->SetValue(wxVariant(value));
}

void UIPropertyInt::ReadFromProperty(const PropertyState& state)
{
    value = state.GetInt();
}


} // /namespace Editor

} // /namespace Fork



// ========================