/*
 * Transform component file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/Component/TransformComponent.h"
#include "Engine/Component/GeometryComponent.h"
#include "Engine/Game/GameObject.h"


namespace Fork
{

namespace Engine
{


TransformComponent::TransformComponent(PropertyFactory* propertyFactory)
{
    CreateProperty< Vector3Property >( PropertyIDs::Position,   propertyFactory );
    CreateProperty< Vector3Property >( PropertyIDs::Rotation,   propertyFactory );
    CreateProperty< Vector3Property >( PropertyIDs::Scale,      propertyFactory );
}

Component::Types TransformComponent::Type() const
{
    return Types::Transform;
}

std::string TransformComponent::IDName() const
{
    return TransformComponent::GetIDName();
}

std::string TransformComponent::GetIDName()
{
    return "transform";
}

void TransformComponent::SetupTransform(const Scene::Transform& transform)
{
    WritePropertyVector3(PropertyIDs::Position, transform.GetPosition());
    WritePropertyVector3(PropertyIDs::Rotation, transform.GetRotation().EulerRotation()*Math::rad2deg);
    WritePropertyVector3(PropertyIDs::Scale, transform.GetScale());

    transform_ = transform;
    isUpToDate_ = true;

    NotifyThisComponentChanged();
}


/*
 * ======= Private: =======
 */

void TransformComponent::OnThisComponentChanged()
{
    if (!isUpToDate_)
    {
        auto position   = ReadPropertyVector3( PropertyIDs::Position );
        auto rotation   = ReadPropertyVector3( PropertyIDs::Rotation );
        auto scale      = ReadPropertyVector3( PropertyIDs::Scale    );

        transform_.SetPosition(position);
        transform_.SetRotation(Math::Quaternionf(Math::Vector3f(rotation)*Math::deg2rad));
        transform_.SetScale(scale);
    }
    else
        isUpToDate_ = false;
}


} // /namespace Engine

} // /namespace Fork



// ========================