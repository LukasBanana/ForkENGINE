/*
 * Editor UI property color file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIPropertyColor.h"
#include "IO/FileSystem/File.h"
#include "../../UIExtensions/UIPropertyGrid.h"
#include "../../WxHelper.h"


namespace Fork
{

namespace Editor
{


UIPropertyColor::UIPropertyColor(UIPropertyGrid& propGrid, const wxString& propName) :
    prop_{ propGrid.FindBoundedProperty<wxColourProperty>(propName) }
{
}

void UIPropertyColor::WriteToProperty()
{
    prop_->SetValue(wxVariant(value));
}

void UIPropertyColor::ReadFromProperty(const PropertyState& state)
{
    value = state.GetColorRGBA();
}


} // /namespace Editor

} // /namespace Fork



// ========================