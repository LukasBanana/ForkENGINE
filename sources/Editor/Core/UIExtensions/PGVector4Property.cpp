/*
 * Editor property-grid vector4 property file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "PGVector4Property.h"
#include "../WxHelper.h"

#include <wx/propgrid/propgrid.h>


WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(WxVector4)


namespace Fork
{

namespace Editor
{


WX_PG_IMPLEMENT_PROPERTY_CLASS(PGVector4Property, wxPGProperty, WxVector4, const WxVector4&, TextCtrl)

PGVector4Property::PGVector4Property(
    const wxString& label, const wxString& name, const WxVector4& value) :
        wxPGProperty(label, name)
{
    SetValue(WXVARIANT(value));
    AddPrivateChild(MakeWxObject<wxFloatProperty>("X", wxPG_LABEL, value.x));
    AddPrivateChild(MakeWxObject<wxFloatProperty>("Y", wxPG_LABEL, value.y));
    AddPrivateChild(MakeWxObject<wxFloatProperty>("Z", wxPG_LABEL, value.z));
    AddPrivateChild(MakeWxObject<wxFloatProperty>("W", wxPG_LABEL, value.w));
}

wxVariant PGVector4Property::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    WxVector4 vec;
    vec << thisValue;

    switch (childIndex)
    {
        case 0: vec.x = childValue.GetDouble(); break;
        case 1: vec.y = childValue.GetDouble(); break;
        case 2: vec.z = childValue.GetDouble(); break;
        case 3: vec.w = childValue.GetDouble(); break;
    }
    
    wxVariant variant;
    variant << vec;
    
    return variant;
}

void PGVector4Property::RefreshChildren()
{
    if (!GetChildCount())
        return;

    const auto& vec = WxVector4RefFromVariant(m_value);

    Item(0)->SetValue(vec.x);
    Item(1)->SetValue(vec.y);
    Item(2)->SetValue(vec.z);
    Item(3)->SetValue(vec.w);
}


} // /namespace Editor

} // /namespace Fork



// ========================