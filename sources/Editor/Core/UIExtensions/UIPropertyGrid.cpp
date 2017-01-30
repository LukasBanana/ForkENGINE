/*
 * Editor UI property grid file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIPropertyGrid.h"
#include "Core/StringModifier.h"
#include "Core/STLHelper.h"
#include "Core/Exception/InvalidArgumentException.h"
#include "../Component/UIComponentFactory.h"
#include "../Component/UIPropertyGridObserver.h"
#include "IO/Core/Log.h"
#include "IO/Core/Console.h"
#include "../../Entity/Entity.h"

#include <list>
#include <algorithm>


namespace Fork
{

namespace Editor
{


/*
 * PropertyDescription structure
 */

UIPropertyGrid::PropertyDescription::PropertyDescription(const PropertyTypes type, const wxString& label, const wxString& name) :
    type    { type  },
    label   { label },
    name    { name  }
{
}
UIPropertyGrid::PropertyDescription::PropertyDescription(const wxString& label, const wxString& name, bool value) :
    type        { PropertyTypes::Bool },
    label       { label               },
    name        { name                },
    valueBool   { value               }
{
}
UIPropertyGrid::PropertyDescription::PropertyDescription(const wxString& label, const wxString& name, float value) :
    type        { PropertyTypes::Float },
    label       { label                },
    name        { name                 },
    valueFloat  { value                }
{
}
UIPropertyGrid::PropertyDescription::PropertyDescription(const wxString& label, const wxString& name, int value, bool hasSpin) :
    type        { PropertyTypes::Int },
    label       { label              },
    name        { name               },
    valueInt    { value              },
    hasSpin     { hasSpin            }
{
}
UIPropertyGrid::PropertyDescription::PropertyDescription(const wxString& label, const wxString& name, int value, long minRange, long maxRange, bool hasSpin) :
    type        { PropertyTypes::RangedInt },
    label       { label                    },
    name        { name                     },
    valueInt    { value                    },
    minRange    { minRange                 },
    maxRange    { maxRange                 },
    hasSpin     { hasSpin                  }
{
}
UIPropertyGrid::PropertyDescription::PropertyDescription(const wxString& label, const wxString& name, const wxColour& value) :
    type        { PropertyTypes::ColorRGB },
    label       { label                   },
    name        { name                    },
    valueColor  { value                   }
{
}
UIPropertyGrid::PropertyDescription::PropertyDescription(const wxString& label, const wxString& name, const wxString& value, const wxString& hint) :
    type        { PropertyTypes::String },
    label       { label                 },
    name        { name                  },
    hint        { hint                  },
    valueString { value                 }
{
}
UIPropertyGrid::PropertyDescription::PropertyDescription(const wxString& label, const wxString& name, const wxArrayString& value, const wxString& hint) :
    type            { PropertyTypes::ArrayString },
    label           { label                      },
    name            { name                       },
    hint            { hint                       },
    valueArrayString{ value                      }
{
}
UIPropertyGrid::PropertyDescription::PropertyDescription(const wxString& label, const wxString& name, const WxVector3& value) :
    type        { PropertyTypes::Vector3 },
    label       { label                  },
    name        { name                   },
    valueVector3{ value                  }
{
}
UIPropertyGrid::PropertyDescription::PropertyDescription(const wxString& label, const wxString& name, const WxVector4& value) :
    type        { PropertyTypes::Vector4 },
    label       { label                  },
    name        { name                   },
    valueVector4{ value                  }
{
}


/* 
 * UIPropertyGrid class
 */

static std::list<UIPropertyGrid*> globalUIPropGridList;

const char UIPropertyGrid::propPrefixSep = '@';

UIPropertyGrid::UIPropertyGrid(
    wxWindow* parent, const wxPoint& position, const wxSize& size, long style) :
    wxPropertyGrid{ parent, wxID_ANY, position, size, style }
{
    /* Bind PG event callbacks */
    Bind(wxEVT_PG_CHANGED, &UIPropertyGrid::OnPGChanged, this);

    /* Add this property grid to the global list */
    globalUIPropGridList.push_back(this);
}
UIPropertyGrid::~UIPropertyGrid()
{
    /* Remove this property grid from the global list */
    RemoveFromList(globalUIPropGridList, this);
}

void UIPropertyGrid::OnApplyProperty(const PropertyState& state)
{
    /* Dummy */
}

void UIPropertyGrid::NotifyPGChanged(wxPGProperty* prop)
{
    if (prop)
        OnPGStateChanged(*prop);
}

wxString UIPropertyGrid::GetID(int id)
{
    return ToStr(UIPropertyGrid::propPrefixSep) + ToStr(id);
}

wxString UIPropertyGrid::GetID(const wxString& prefix, int id)
{
    return prefix + ToStr(UIPropertyGrid::propPrefixSep) + ToStr(id);
}

wxString UIPropertyGrid::GetID(const wxString& prefix, const wxString& key)
{
    return prefix + ToStr(UIPropertyGrid::propPrefixSep) + key;
}

int UIPropertyGrid::ExtractPropertyID(const wxString& name)
{
    const auto len = name.size();

    if (len < 2)
        return -1;

    /* Search ID separator 'UIPropertyGrid::propPrefixSep' from right-to-left */
    size_t num = 0;
    for (size_t pos = len; pos > 0; --pos)
    {
        if (name[pos - 1] == UIPropertyGrid::propPrefixSep)
            return StrToNum<int>(name.Right(num).ToStdString());
        ++num;
    }

    return -1;
}

wxString UIPropertyGrid::ExtractPropertyKey(const wxString& name)
{
    const auto len = name.size();

    if (len < 2)
        return name;

    /* Search key separator 'UIPropertyGrid::propPrefixSep' from right-to-left */
    size_t num = 0;
    for (size_t pos = len; pos > 0; --pos)
    {
        if (name[pos - 1] == UIPropertyGrid::propPrefixSep)
            return name.Right(num).ToStdString();
        ++num;
    }

    return name;
}

bool UIPropertyGrid::ExtractPropertyIDAndPrefix(const wxString& name, int& id, wxString& prefix)
{
    const auto len = name.size();

    if (len < 2)
        return false;

    /* Search ID separator 'UIPropertyGrid::propPrefixSep' from right-to-left */
    size_t num = 0;
    for (size_t pos = len; pos > 0; --pos)
    {
        if (name[pos - 1] == UIPropertyGrid::propPrefixSep)
        {
            id = StrToNum<int>(name.Right(num).ToStdString());
            prefix = name.Left(pos - 1);
            return true;
        }
        ++num;
    }

    id = -1;

    return false;
}

/* --- Property category --- */

wxPropertyCategory* UIPropertyGrid::AddPropertyCategory(
    const wxString &label, const wxString& name, wxPGProperty* parent)
{
    wxPropertyCategory* prop = new wxPropertyCategory(label, name);
    
    if (parent)
        AppendIn(parent, prop);
    else
        Append(prop);
    
    categoryProperties_.push_back(prop);

    return prop;
}

/* --- Properties --- */

wxBoolProperty* UIPropertyGrid::AddBoolProperty(
    const wxString& label, const wxString& name, bool value)
{
    auto prop = CreateProperty<wxBoolProperty>(label, name, value);

    prop->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);

    return prop;
}

wxFloatProperty* UIPropertyGrid::AddFloatProperty(
    const wxString& label, const wxString& name, float value)
{
    return CreateProperty<wxFloatProperty>(label, name, value);
}

wxIntProperty* UIPropertyGrid::AddIntProperty(
    const wxString& label, const wxString& name, int value, bool hasSpin)
{
    auto prop = CreateProperty<wxIntProperty>(label, name, value);

    if (hasSpin)
        prop->SetEditor(wxPGEditor_SpinCtrl);

    return prop;
}

wxIntProperty* UIPropertyGrid::AddIntProperty(
    const wxString& label, const wxString& name, int value, long minRange, long maxRange, bool hasSpin)
{
    auto prop = CreateProperty<wxIntProperty>(label, name, value);

    if (hasSpin)
        prop->SetEditor(wxPGEditor_SpinCtrl);

    prop->SetAttribute(wxPG_ATTR_MIN, minRange);
    prop->SetAttribute(wxPG_ATTR_MAX, maxRange);

    return prop;
}

wxUIntProperty* UIPropertyGrid::AddUIntProperty(
    const wxString& label, const wxString& name, unsigned int value)
{
    return CreateProperty<wxUIntProperty>(label, name, value);
}

wxColourProperty* UIPropertyGrid::AddColorRGBProperty(
    const wxString& label, const wxString& name, const wxColour& value)
{
    return CreateProperty<wxColourProperty>(label, name, value);
}

wxColourProperty* UIPropertyGrid::AddColorRGBAProperty(
    const wxString& label, const wxString& name, const wxColour& value)
{
    auto prop = CreateProperty<wxColourProperty>(label, name, value);

    prop->SetAttribute(wxPG_COLOUR_HAS_ALPHA, true);

    return prop;
}

wxSystemColourProperty* UIPropertyGrid::AddSystemColorProperty(
    const wxString& label, const wxString& name, const wxColour& value)
{
    return CreateProperty<wxSystemColourProperty>(label, name, value);
}

wxEnumProperty* UIPropertyGrid::AddEnumProperty(
    const wxString& label, const wxString& name)
{
    return CreateProperty<wxEnumProperty>(label, name);
}

wxEnumProperty* UIPropertyGrid::AddEnumProperty(
    const wxString& label, const wxString& name, wxPGChoices& choices, int value)
{
    return CreateProperty<wxEnumProperty>(label, name, choices, value);
}

wxStringProperty* UIPropertyGrid::AddStringProperty(
    const wxString& label, const wxString& name, const wxString& value, const wxString& hint)
{
    auto prop = CreateProperty<wxStringProperty>(label, name, value);

    if (!hint.empty())
        prop->SetAttribute(wxPG_ATTR_HINT, hint);

    return prop;
}

wxLongStringProperty* UIPropertyGrid::AddLongStringProperty(
    const wxString& label, const wxString& name, const wxString& value, const wxString& hint)
{
    auto prop = CreateProperty<wxLongStringProperty>(label, name, value);

    if (!hint.empty())
        prop->SetAttribute(wxPG_ATTR_HINT, hint);

    return prop;
}

wxArrayStringProperty* UIPropertyGrid::AddArrayStringProperty(
    const wxString& label, const wxString& name, const wxArrayString& value, const wxString& hint)
{
    auto prop = CreateProperty<wxArrayStringProperty>(label, name, value);

    if (!hint.empty())
        prop->SetAttribute(wxPG_ATTR_HINT, hint);

    return prop;
}

wxFileProperty* UIPropertyGrid::AddFileProperty(
    const wxString& label, const wxString& name, const wxString& value, const wxString& hint,
    long style, const wxString& dialogLabel, const wxString& wildCard, const wxString& initialPath)
{
    auto prop = CreateProperty<wxFileProperty>(label, name, value);

    if (!hint.empty())
        prop->SetAttribute(wxPG_ATTR_HINT, hint);
    if (style)
        prop->SetAttribute(wxPG_FILE_DIALOG_STYLE, style);
    if (!dialogLabel.empty())
        prop->SetAttribute(wxPG_FILE_DIALOG_TITLE, dialogLabel);
    if (!wildCard.empty())
        prop->SetAttribute(wxPG_FILE_WILDCARD, wildCard);
    if (!initialPath.empty())
        prop->SetAttribute(wxPG_FILE_INITIAL_PATH, initialPath);

    return prop;
}

wxDirProperty* UIPropertyGrid::AddDirProperty(
    const wxString& label, const wxString& name, const wxString& value, const wxString& hint, const wxString& dialogLabel)
{
    auto prop = CreateProperty<wxDirProperty>(label, name, value);

    if (!hint.empty())
        prop->SetAttribute(wxPG_ATTR_HINT, hint);
    if (!dialogLabel.empty())
        prop->SetAttribute(wxPG_DIR_DIALOG_MESSAGE, dialogLabel);

    return prop;
}

PGVector3Property* UIPropertyGrid::AddVector3Property(const wxString& label, const wxString& name, const WxVector3& value)
{
    return CreateProperty<PGVector3Property>(label, name, value);
}

PGVector4Property* UIPropertyGrid::AddVector4Property(const wxString& label, const wxString& name, const WxVector4& value)
{
    return CreateProperty<PGVector4Property>(label, name, value);
}

/* --- Setter --- */

void UIPropertyGrid::SetPropertyBool(const wxString& name, bool value)
{
    SetProperty(name, value);
}

void UIPropertyGrid::SetPropertyInt(const wxString& name, int value)
{
    SetProperty(name, value);
}

void UIPropertyGrid::SetPropertyFloat(const wxString& name, float value)
{
    SetProperty(name, value);
}

void UIPropertyGrid::SetPropertyEnum(const wxString& name, int value)
{
    SetProperty(name, value);
}

void UIPropertyGrid::SetPropertyEnum(const wxString& name, const wxString& value)
{
    SetProperty(name, value);
}

void UIPropertyGrid::SetPropertyString(const wxString& name, const wxString& value)
{
    SetProperty(name, value);
}

void UIPropertyGrid::SetPropertyVector3(const wxString& name, const WxVector3& value)
{
    SetProperty(name, value);
}

void UIPropertyGrid::SetPropertyVector4(const wxString& name, const WxVector4& value)
{
    SetProperty(name, value);
}

void UIPropertyGrid::SetPropertyColor(const wxString& name, const wxColour& value)
{
    SetProperty(name, value);
}

/* --- Getter --- */

#define PGPROP_GET(func, def)               \
    auto prop = GetPropertyByName(name);    \
    if (prop)                               \
        return prop->GetValue().func();     \
    return def

bool UIPropertyGrid::GetPropertyBool(const wxString& name) const
{
    PGPROP_GET(GetBool, false);
}

int UIPropertyGrid::GetPropertyInt(const wxString& name) const
{
    PGPROP_GET(GetLong, 0);
}

float UIPropertyGrid::GetPropertyFloat(const wxString& name) const
{
    PGPROP_GET(GetDouble, 0.0f);
}

int UIPropertyGrid::GetPropertyEnum(const wxString& name) const
{
    PGPROP_GET(GetLong, 0);
}

wxString UIPropertyGrid::GetPropertyString(const wxString& name) const
{
    PGPROP_GET(GetString, "");
}

#undef PGPROP_GET

WxVector3 UIPropertyGrid::GetPropertyVector3(const wxString& name) const
{
    auto prop = GetPropertyByName(name);
    
    if (prop)
    {
        auto value = prop->GetValue();
        return WxVector3RefFromVariant(value);
    }
    
    return WxVector3();
}

WxVector4 UIPropertyGrid::GetPropertyVector4(const wxString& name) const
{
    auto prop = GetPropertyByName(name);
    
    if (prop)
    {
        auto value = prop->GetValue();
        return WxVector4RefFromVariant(value);
    }
    
    return WxVector4();
}

wxColour UIPropertyGrid::GetPropertyColor(const wxString& name) const
{
    auto prop = GetPropertyByName(name);
    
    if (prop)
    {
        wxVariant value = prop->GetValue();
        
        wxColour color;
        color << value;
        
        return color;
    }
    
    return wxColour();
}

/* --- Common --- */

void UIPropertyGrid::HideAllCategories()
{
    for (auto prop : categoryProperties_)
        prop->Hide(true);
}

void UIPropertyGrid::ShowProperty(const wxString& name, bool show)
{
    auto prop = GetPropertyByName(name);
    if (prop)
        prop->Hide(!show);
}

void UIPropertyGrid::AddProperties(const std::initializer_list<PropertyDescription>& propDescList)
{
    for (auto desc : propDescList)
    {
        switch (desc.type)
        {
            case PropertyTypes::Category:
                AddPropertyCategory(desc.label, desc.name);
                break;

            case PropertyTypes::Bool:
                AddBoolProperty(desc.label, desc.name, desc.valueBool);
                break;

            case PropertyTypes::Float:
                AddFloatProperty(desc.label, desc.name, desc.valueFloat);
                break;

            case PropertyTypes::Int:
                AddIntProperty(desc.label, desc.name, desc.valueInt, desc.hasSpin);
                break;

            case PropertyTypes::RangedInt:
                AddIntProperty(desc.label, desc.name, desc.valueInt, desc.minRange, desc.maxRange, desc.hasSpin);
                break;
            case PropertyTypes::UInt:
                AddUIntProperty(desc.label, desc.name, desc.valueUInt);
                break;

            case PropertyTypes::ColorRGB:
                AddColorRGBProperty(desc.label, desc.name, desc.valueColor);
                break;
            case PropertyTypes::ColorRGBA:
                AddColorRGBAProperty(desc.label, desc.name, desc.valueColor);
                break;
            case PropertyTypes::SystemColor:
                AddSystemColorProperty(desc.label, desc.name, desc.valueColor);
                break;

            /*case PropertyTypes::Enum:
                AddEnumProperty(desc.label, desc.name, desc.choices, desc.valueInt);
                break;*/

            case PropertyTypes::String:
                AddStringProperty(desc.label, desc.name, desc.valueString, desc.hint);
                break;
            case PropertyTypes::LongString:
                AddLongStringProperty(desc.label, desc.name, desc.valueString, desc.hint);
                break;
            case PropertyTypes::ArrayString:
                AddArrayStringProperty(desc.label, desc.name, desc.valueArrayString, desc.hint);
                break;

            case PropertyTypes::Vector3:
                AddVector3Property(desc.label, desc.name, desc.valueVector3);
                break;
            case PropertyTypes::Vector4:
                AddVector4Property(desc.label, desc.name, desc.valueVector4);
                break;

            default:
                break;
        }
    }
}

void UIPropertyGrid::ChangeColorScheme(const ColorScheme& scheme)
{
    SetLineColour               ( scheme.lineColor          );
    SetCellBackgroundColour     ( scheme.cellBgColor        );
    SetEmptySpaceColour         ( scheme.cellBgColor        );

    SetCaptionBackgroundColour  ( scheme.captionBgColor     );
    SetMarginColour             ( scheme.captionBgColor     );

    SetCellDisabledTextColour   ( scheme.disabledTextColor  );

    SetSelectionBackgroundColour( scheme.selectionBgColor   );
    SetSelectionTextColour      ( scheme.selectionTextColor );

    SetCaptionTextColour        ( scheme.captionTextColor   );
    SetCellTextColour           ( scheme.cellTextColor      );
}

void UIPropertyGrid::DebugPropertyNames()
{
    static const IO::Log::ColorFlags debugColor = (Platform::ConsoleManip::Colors::Pink | Platform::ConsoleManip::Colors::Intens);

    IO::Log::Message("UIPropertyGrid::DebugPropertyNames", debugColor);
    IO::Log::ScopedIndent indent;

    for (auto it = GetIterator(); !it.AtEnd(); ++it)
    {
        auto prop = *it;
        IO::Log::Message(prop->GetName().ToStdString(), debugColor);
    }
}

void UIPropertyGrid::ChangeColorSchemeForAll(const ColorScheme& scheme)
{
    for (auto propGrid : globalUIPropGridList)
        propGrid->ChangeColorScheme(scheme);
}


/*
 * ======= Private: =======
 */

/*
This function is called when a property has changed.
All observers of this property-grid will be notified about that change.
*/
void UIPropertyGrid::OnPGChanged(wxPropertyGridEvent& event)
{
    OnPGStateChanged(event);
}

void UIPropertyGrid::OnPGStateChanged(const PropertyState& state)
{
    /* Extract property ID and name prefix */
    int id = 0;
    wxString prefix;
    state.ExtractPropertyIDAndPrefix(id, prefix);

    /* Call 'apply property' callback */
    OnApplyProperty(state);

    try
    {
        /* Get component type by property prefix */
        const auto componentType = UIComponentFactory::QueryTypeByNamePrefix(prefix.ToStdString());

        /* Notify all observers that this property-grid has changed */
        NotifyAllObservers(componentType, state);
    }
    catch (const InvalidArgumentException& err)
    {
        IO::Log::Error(err);
    }
}

void UIPropertyGrid::NotifyObserver(
    UIPropertyGridObserver& observer, const Engine::Component::Types componentType, const PropertyState& propertyState) const
{
    observer.OnPGChanged(componentType, propertyState);
}


} // /namespace Editor

} // /namespace Fork



// ========================