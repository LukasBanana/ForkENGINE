/*
 * Editor UI property vector3 file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIPropertyVector3.h"
#include "IO/FileSystem/File.h"
#include "../../UIExtensions/UIPropertyGrid.h"


namespace Fork
{

namespace Editor
{


UIPropertyVector3::UIPropertyVector3(UIPropertyGrid& propGrid, const wxString& propName) :
    prop_{ propGrid.FindBoundedProperty<PGVector3Property>(propName) }
{
}

void UIPropertyVector3::WriteToProperty()
{
    prop_->SetValue(wxVariant(WxVector3(value)));
}

void UIPropertyVector3::ReadFromProperty(const PropertyState& state)
{
    value = state.GetVector3();
}


} // /namespace Editor

} // /namespace Fork



// ========================