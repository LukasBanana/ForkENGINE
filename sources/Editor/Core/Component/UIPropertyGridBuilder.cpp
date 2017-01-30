/*
 * Editor UI property grid builder file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIPropertyGridBuilder.h"
#include "../../Core/UIExtensions/UIPropertyGrid.h"
#include "../../Language/LanguageLoader.h"


namespace Fork
{

namespace Editor
{


UIPropertyGridBuilder::UIPropertyGridBuilder(UIPropertyGrid& propGrid) :
    propGrid_{ &propGrid }
{
}

void UIPropertyGridBuilder::InitCategory(const StringID labelStringID, const std::string& namePrefix)
{
    namePrefix_ = namePrefix;
    keyCounter_ = 0;
    propGrid_->AddPropertyCategory(Language::String(labelStringID), namePrefix_);
}

/* --- Properties --- */

void UIPropertyGridBuilder::Bool(const StringID labelStringID)
{
    propGrid_->AddBoolProperty(Language::String(labelStringID), NextPropID());
}

void UIPropertyGridBuilder::Float(const StringID labelStringID)
{
    propGrid_->AddFloatProperty(Language::String(labelStringID), NextPropID());
}

void UIPropertyGridBuilder::Int(const StringID labelStringID, bool hasSpin)
{
    propGrid_->AddIntProperty(Language::String(labelStringID), NextPropID(), 0, hasSpin);
}

void UIPropertyGridBuilder::Int(const StringID labelStringID, long minRange, long maxRange, bool hasSpin)
{
    propGrid_->AddIntProperty(Language::String(labelStringID), NextPropID(), 0, minRange, maxRange, hasSpin);
}

void UIPropertyGridBuilder::UInt(const StringID labelStringID)
{
    propGrid_->AddUIntProperty(Language::String(labelStringID), NextPropID());
}

void UIPropertyGridBuilder::ColorRGB(const StringID labelStringID)
{
    propGrid_->AddColorRGBProperty(Language::String(labelStringID), NextPropID());
}

void UIPropertyGridBuilder::ColorRGBA(const StringID labelStringID)
{
    propGrid_->AddColorRGBAProperty(Language::String(labelStringID), NextPropID());
}

void UIPropertyGridBuilder::SystemColor(const StringID labelStringID)
{
    propGrid_->AddSystemColorProperty(Language::String(labelStringID), NextPropID());
}

void UIPropertyGridBuilder::Enum(const StringID labelStringID, wxPGChoices& choices)
{
    propGrid_->AddEnumProperty(Language::String(labelStringID), NextPropID(), choices);
}

void UIPropertyGridBuilder::Enum(const StringID labelStringID)
{
    propGrid_->AddEnumProperty(Language::String(labelStringID), NextPropID());
}

void UIPropertyGridBuilder::String(const StringID labelStringID)
{
    propGrid_->AddStringProperty(Language::String(labelStringID), NextPropID());
}

void UIPropertyGridBuilder::LongString(const StringID labelStringID)
{
    propGrid_->AddLongStringProperty(Language::String(labelStringID), NextPropID());
}

void UIPropertyGridBuilder::ArrayString(const StringID labelStringID)
{
    propGrid_->AddArrayStringProperty(Language::String(labelStringID), NextPropID());
}

void UIPropertyGridBuilder::File(const StringID labelStringID)
{
    propGrid_->AddFileProperty(Language::String(labelStringID), NextPropID());
}

void UIPropertyGridBuilder::Dir(const StringID labelStringID)
{
    propGrid_->AddDirProperty(Language::String(labelStringID), NextPropID());
}

void UIPropertyGridBuilder::Vector3(const StringID labelStringID)
{
    propGrid_->AddVector3Property(Language::String(labelStringID), NextPropID());
}

void UIPropertyGridBuilder::Vector4(const StringID labelStringID)
{
    propGrid_->AddVector4Property(Language::String(labelStringID), NextPropID());
}

void UIPropertyGridBuilder::Asset(const StringID labelStringID)
{
    propGrid_->AddStringProperty(Language::String(labelStringID), NextPropID());
}


/*
 * ======= Private: =======
 */

wxString UIPropertyGridBuilder::NextPropID()
{
    return UIPropertyGrid::GetID(namePrefix_, keyCounter_++);
}


} // /namespace Editor

} // /namespace Fork



// ========================