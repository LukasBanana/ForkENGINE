/*
 * Notification component file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/Component/NotificationComponent.h"
#include "Engine/Game/GameObject.h"


namespace Fork
{

namespace Engine
{


NotificationComponent::NotificationComponent(GameObject* owner) :
    owner_{ owner }
{
}

Component::Types NotificationComponent::Type() const
{
    return Types::Notification;
}

std::string NotificationComponent::IDName() const
{
    return NotificationComponent::GetIDName();
}

std::string NotificationComponent::GetIDName()
{
    return "notification";
}


/*
 * ======= Private: =======
 */

bool NotificationComponent::IsObservableOfInterest(const Types type) const
{
    /* Always return true, an entity wants to be notified about all components */
    return true;
}

void NotificationComponent::OnObservableChanged(const Component* observable)
{
    /* Notify owner of this component */
    owner_->OnComponentChanged(observable);
}


} // /namespace Engine

} // /namespace Fork



// ========================