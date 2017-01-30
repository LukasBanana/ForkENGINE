/*
 * Editor property-grid vector3 property file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "PGVector3Property.h"
#include "../WxHelper.h"

#include <wx/propgrid/propgrid.h>


WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(WxVector3)


namespace Fork
{

namespace Editor
{


WX_PG_IMPLEMENT_PROPERTY_CLASS(PGVector3Property, wxPGProperty, WxVector3, const WxVector3&, TextCtrl)

PGVector3Property::PGVector3Property(
    const wxString& label, const wxString& name, const WxVector3& value) :
        wxPGProperty(label, name)
{
    SetValue(WXVARIANT(value));
    AddPrivateChild(MakeWxObject<wxFloatProperty>("X", wxPG_LABEL, value.x));
    AddPrivateChild(MakeWxObject<wxFloatProperty>("Y", wxPG_LABEL, value.y));
    AddPrivateChild(MakeWxObject<wxFloatProperty>("Z", wxPG_LABEL, value.z));
}

wxVariant PGVector3Property::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    WxVector3 vec;
    vec << thisValue;

    switch (childIndex)
    {
        case 0: vec.x = childValue.GetDouble(); break;
        case 1: vec.y = childValue.GetDouble(); break;
        case 2: vec.z = childValue.GetDouble(); break;
    }
    
    wxVariant variant;
    variant << vec;
    
    return variant;
}

void PGVector3Property::RefreshChildren()
{
    if (!GetChildCount())
        return;

    const auto& vec = WxVector3RefFromVariant(m_value);

    Item(0)->SetValue(vec.x);
    Item(1)->SetValue(vec.y);
    Item(2)->SetValue(vec.z);
}


} // /namespace Editor

} // /namespace Fork



// ========================