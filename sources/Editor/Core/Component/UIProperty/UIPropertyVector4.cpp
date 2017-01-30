/*
 * Editor UI property vector4 file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIPropertyVector4.h"
#include "IO/FileSystem/File.h"
#include "../../UIExtensions/UIPropertyGrid.h"


namespace Fork
{

namespace Editor
{


UIPropertyVector4::UIPropertyVector4(UIPropertyGrid& propGrid, const wxString& propName) :
    prop_{ propGrid.FindBoundedProperty<PGVector4Property>(propName) }
{
}

void UIPropertyVector4::WriteToProperty()
{
    prop_->SetValue(wxVariant(WxVector4(value)));
}

void UIPropertyVector4::ReadFromProperty(const PropertyState& state)
{
    value = state.GetVector4();
}


} // /namespace Editor

} // /namespace Fork



// ========================