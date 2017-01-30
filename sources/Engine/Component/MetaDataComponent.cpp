/*
 * Meta data component file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/Component/MetaDataComponent.h"
#include "Engine/Component/GeometryComponent.h"
#include "Engine/Game/GameObject.h"


namespace Fork
{

namespace Engine
{


MetaDataComponent::MetaDataComponent(PropertyFactory* propertyFactory)
{
    CreateProperty< Component::StringProperty >( PropertyIDs::Name,     propertyFactory )->value = "Entity";
    CreateProperty< Component::BoolProperty   >( PropertyIDs::Enabled,  propertyFactory )->value = true;
}

Component::Types MetaDataComponent::Type() const
{
    return Types::MetaData;
}

std::string MetaDataComponent::IDName() const
{
    return MetaDataComponent::GetIDName();
}

std::string MetaDataComponent::GetIDName()
{
    return "meta_data";
}

void MetaDataComponent::SetupName(const std::string& name)
{
    WritePropertyString(PropertyIDs::Name, name);
    NotifyThisComponentChanged();
}


} // /namespace Engine

} // /namespace Fork



// ========================