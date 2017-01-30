/*
 * Game object file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/Game/GameObject.h"
#include "Engine/Component/ComponentFactory.h"
#include "Engine/Component/ComponentException.h"
#include "Scene/Node/DynamicSceneNode.h"
#include "Core/STLHelper.h"

#include "Engine/Component/NotificationComponent.h"
#include "Engine/Component/GeometryComponent.h"

#include <algorithm>


namespace Fork
{

namespace Engine
{


GameObject::GameObject(const std::shared_ptr<IdentityFactory>& idFactory) :
    Identifiable{ idFactory }
{
}
GameObject::~GameObject()
{
}

/* --- Common --- */

std::unique_ptr<GameObject> GameObject::InstantiateCopy(
    const std::shared_ptr<IdentityFactory>& idFactory, ComponentFactory* componentFactory) const
{
    auto copy = std::make_unique<GameObject>(idFactory);

    for (const auto& comp : GetComponents())
        copy->components_.push_back(comp->InstantiateCopy(componentFactory));

    return copy;
}

/* --- I/O system --- */

bool GameObject::LoadContent(IO::File& file)
{
    //todo
    return false;
}

bool GameObject::SaveContent(IO::File& file)
{
    //todo
    return false;
}

bool GameObject::LoadState(IO::File& file)
{
    //todo
    return false;
}

bool GameObject::SaveState(IO::File& file)
{
    //todo
    return false;
}

/* --- Components --- */

Component* GameObject::CreateComponent(const Component::Types type)
{
    /* Check if component type does not already exists */
    if (FindComponent(type))
        throw ComponentException(__FUNCTION__, ComponentException::Failures::ComponentAlreadyExists);

    /* Get component factory */
    static ComponentFactory defaultFactory;

    auto factory = GetComponentFactory();
    if (!factory)
        factory = &defaultFactory;

    /* Instantiate new component */
    std::unique_ptr<Component> comp;
    
    if (type == Component::Types::Notification)
        comp = std::make_unique<NotificationComponent>(this);
    else
        comp = factory->InstantiateComponent(type);

    /* Register component and add to the list */
    auto compRef = comp.get();
    RegisterComponent(compRef);

    /* Add component to the list */
    components_.push_back(std::move(comp));

    return compRef;
}

void GameObject::DeleteComponent(const Component::Types type)
{
    /* Find component in the list with the specified type */
    auto it = std::find_if(
        components_.begin(),
        components_.end(),
        [&type](const std::unique_ptr<Component>& component)
        {
            return component->Type() == type;
        }
    );

    if (it != components_.end())
    {
        /*
        Temporarily store object to avoid early deletion,
        then remove from list and call "UnregisterComponent".
        */
        auto comp = std::move(*it);
        components_.erase(it);
        UnregisterComponent(comp.get());
    }
}

Component* GameObject::FindComponent(const Component::Types type) const
{
    auto it = std::find_if(
        components_.begin(),
        components_.end(),
        [&type](const std::unique_ptr<Component>& component)
        {
            return component->Type() == type;
        }
    );
    return (it != components_.end() ? it->get() : nullptr);
}

void GameObject::ForEachComponentWithSceneNode(const ComponentWithSceneNodeProc& callback)
{
    if (callback)
    {
        for (const auto& comp : GetComponents())
        {
            switch (comp->Type())
            {
                case Component::Types::Geometry:
                    callback(comp.get(), static_cast<GeometryComponent*>(comp.get())->GetSceneNode());
                    break;
                default:
                    break;
            }
        }
    }
}


/*
 * ======= Protected: =======
 */

ComponentFactory* GameObject::GetComponentFactory() const
{
    return nullptr; // dummy
}

void GameObject::OnRegisterComponent(Component* comp)
{
    // dummy
}

void GameObject::OnUnregisterComponent(Component* comp)
{
    // dummy
}

void GameObject::OnComponentChanged(const Component* comp)
{
    // dummy
}


/*
 * ======= Private: =======
 */

void GameObject::RegisterComponent(Component* comp)
{
    for (auto& observer : components_)
    {
        /* Register new component as observable to established component */
        observer->RegisterObservable(*comp);

        /* Register established component as observable to the new component */
        comp->RegisterObservable(*observer);
    }

    /* Callback */
    OnRegisterComponent(comp);
}

void GameObject::UnregisterComponent(Component* comp)
{
    /* Callback */
    OnUnregisterComponent(comp);

    for (auto& observer : components_)
    {
        /* Unregister new component as observable to established component */
        observer->UnregisterObservable(*comp);

        /* Unregister established component as observable to the new component */
        comp->UnregisterObservable(*observer);
    }
}


} // /namespace Engine

} // /namespace Fork



// ========================