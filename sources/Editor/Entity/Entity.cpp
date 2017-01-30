/*
 * Editor entity file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Entity.h"
#include "EntitySelector.h"
#include "Engine/Component/GeometryComponent.h"
#include "Core/Exception/NullPointerException.h"
#include "../Frame/Main/MainFrameSceneGraphController.h"
#include "../Frame/Main/MainFrameComponentController.h"
#include "../Frame/Main/MainFrame.h"
#include "../Tools/AssetBrowser/Asset/GeometryHierarchyBuilder.h"
#include "../Core/Devices.h"
#include "../Core/Component/UIComponentFactory.h"
#include "../Core/UIExtensions/UIPropertyGrid.h"

#include "../Core/Component/MetaDataUIComponent.h"
#include "../Core/Component/TransformUIComponent.h"
#include "../Core/Component/GeometryUIComponent.h"


namespace Fork
{

namespace Editor
{


using namespace Devices;
using namespace Engine;

static UITreeView* GetSceneGraphTreeView()
{
    return MainFrame::Instance()->GetSceneGraphCtrl()->GetTreeView();
}

Entity::Entity(const std::shared_ptr<IdentityFactory>& idFactory) :
    ComponentContainer{ idFactory }
{
    CreateComponent(Component::Types::Notification);

    /* Create tree view entry */
    treeViewEntry_ = GetSceneGraphTreeView()->CreateEntry("GameObject");
}
Entity::~Entity()
{
    /* Remove entity from selection list */
    entitySelector->GetModel()->RemoveNode(this);
}

const Scene::Transform& Entity::LocalTransform() const
{
    return transform_;
}

Math::Matrix4f Entity::GlobalTransform() const
{
    return transform_.GetMatrix();
}

/*
This function (callback) is called when the selector transformed this entity,
e.g. when the user moved or rotated an object inside the editor.
*/
void Entity::OnTransform()
{
    /* Update transform component */
    auto comp = FindComponent(Component::Types::Transform);
    if (comp)
        static_cast<TransformComponent*>(comp)->SetupTransform(transform_);
}

void Entity::OnTransformEnd()
{
    /* Update transform component */
    for (auto component : GetUIComponents())
    {
        if (component->EngineComponent()->Type() == Component::Types::Transform)
        {
            if (IsActiveSelection())
                component->WriteToPropertyGrid();
            break;
        }
    }
}

/* --- Components --- */

void Entity::SelectAndShowUIComponents()
{
    /* Select this entity */
    Select();

    /* Show components */
    ShowUIComponents();
}

bool Entity::IsActiveSelection() const
{
    return Devices::entitySelector->GetActiveEntity() == this;
}

void Entity::UpdateSelectionCollider()
{
    auto comp = FindComponent(Component::Types::Geometry);
    if (comp)
    {
        /* Update collision body for selecting this entity */
        auto geometryComp = static_cast<GeometryComponent*>(comp);
        auto collider = physicsWorld->CreateMeshCollider(
            *geometryComp->GetSceneNode()->geometry, nullptr, false
        );
        UpdateCollisionBody(collider);
    }
}


/*
 * ======= Private: =======
 */

void Entity::UpdateCollisionBody(Physics::ColliderPtr collider)
{
    if (physicsWorld)
    {
        /* Create collider and collision static body */
        collisionBody_              = physicsWorld->CreateStaticBody(collider);

        /* Refer with the user data to this entity instance */
        collisionBody_->userData    = this;

        /* Refer with the static object's transformation to the scene node's transformation */
        collisionBody_->transform   = (&transform_);

        UpdateCollisionBodyTransform();
    }
}

void Entity::UpdateCollisionBodyTransform()
{
    if (collisionBody_)
        collisionBody_->SetupTransform(transform_);
}

Scene::Transform& Entity::GetTransform()
{
    return transform_;
}


/*
 * ======= Protected: =======
 */

/*
This is called whenever a component property has changed.
Responsible for this is the "NotificationComponent".
*/
void Entity::OnComponentChanged(const Component* comp)
{
    switch (comp->Type())
    {
        case Component::Types::MetaData:
        {
            /* Update entry in scene graph view for this entity, when the name has change */
            auto metaDataComp = static_cast<const MetaDataComponent*>(comp);
            treeViewEntry_->ChangeText(metaDataComp->ReadPropertyString(MetaDataComponent::PropertyIDs::Name));
        }
        break;

        case Component::Types::Transform:
        {
            /* Transform this entity when the transform component changed */
            auto transformComp = static_cast<const TransformComponent*>(comp);
            transform_ = transformComp->GetTransform();
            UpdateCollisionBodyTransform();
        }
        break;

        case Component::Types::Geometry:
        {
            /* Build tree view hierarchy for the geometry */
            auto geometryComp = static_cast<const GeometryComponent*>(comp);
            if (geometryComp->GetSceneNode()->geometry)
            {
                GeometryHierarchyBuilder builder;
                builder.BuildHierarchy(
                    treeViewEntry_->GetTreeView(),
                    geometryComp->GetSceneNode()->geometry.get(),
                    treeViewEntry_->GetItem()
                );
            }
        }
        break;
    }
}


} // /namespace Editor

} // /namespace Fork



// ========================