/*
 * Editor property state file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "PropertyState.h"
#include "UIPropertyGrid.h"
#include "../WxHelper.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace Editor
{


PropertyState::PropertyState(const wxPropertyGridEvent& event) :
    name{ event.GetPropertyName() },
    key { ExtractKey()            }
{
    Init(event.GetValue());
}
PropertyState::PropertyState(wxPGProperty& prop) :
    name{ prop.GetName() },
    key { ExtractKey()   }
{
    Init(prop.GetValue());
}

int PropertyState::ExtractPropertyID() const
{
    return UIPropertyGrid::ExtractPropertyID(name);
}

PropertyState::PropertyKey PropertyState::ExtractKey() const
{
    return StrToNum<PropertyKey>(UIPropertyGrid::ExtractPropertyKey(name).ToStdString());
}

bool PropertyState::ExtractPropertyIDAndPrefix(int& id, wxString& prefix) const
{
    return UIPropertyGrid::ExtractPropertyIDAndPrefix(name, id, prefix);
}


/*
 * ======= Private: =======
 */

void PropertyState::Init(const wxVariant& variant)
{
    /* Get property name and its value */
    const wxString variantType = variant.GetType();
    
    /* Get all possible values */
    if (variantType == "bool")
        Write(variant.GetBool());
    else if (variantType == "double")
        Write(static_cast<float>(variant.GetDouble()));
    else if (variantType == "long")
        Write(variant.GetLong());
    else if (variantType == "ulonglong")
        Write(static_cast<unsigned int>(variant.GetULongLong().GetValue()));
    else if (variantType == "string")
        Write(variant.GetString().ToStdString());
    else if (variantType == "wxColour")
    {
        wxColour color;
        color << variant;
        Write(WxColorToRGBA<unsigned char>(color));
    }
    else if (variantType == "WxVector3")
        Write(Math::Vector3f(WxVector3RefFromVariant(variant)));
    else if (variantType == "WxVector4")
        Write(Math::Vector4f(WxVector4RefFromVariant(variant)));
}


} // /namespace Editor

} // /namespace Fork



// ========================