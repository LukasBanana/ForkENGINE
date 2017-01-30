/*
 * Editor entity header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_ENTITY_H__
#define __FORK_EDITOR_ENTITY_H__


#include "Core/DeclPtr.h"
#include "../Core/Component/ComponentContainer.h"
#include "Scene/Node/DynamicSceneNode.h"
#include "Physics/Dynamic/StaticBody.h"
#include "Utility/Selector/Selectable.h"
#include "../Core/UIExtensions/UITreeView.h"

#include <map>


namespace Fork
{

namespace Editor
{


class EntityComponentTransform;
class UIPropertyGrid;

DECL_SHR_PTR(Entity);

//! Base class for all entities in the editor view.
class Entity final : public ComponentContainer, public Utility::Selectable
{
    
    public:
        
        Entity(const std::shared_ptr<IdentityFactory>& idFactory);
        virtual ~Entity();

        /* --- <Selectable> interface --- */

        const Scene::Transform& LocalTransform() const override;
        Math::Matrix4f GlobalTransform() const override;

        void OnTransform() override;
        void OnTransformEnd() override;

        /* --- Components --- */

        void SelectAndShowUIComponents();

        /**
        Returns true if this is the active selected entity.
        \remarks This is a helper function to compare 'this' with 'EntitySelector::ActiveEntity()'.
        \see EntitySelector::ActiveEntity
        */
        bool IsActiveSelection() const;

        //! Updates the physics collider for the entity selection.
        void UpdateSelectionCollider();

        /* === Inline functions === */

        //! Returns the physics collider body.
        inline Physics::StaticBody* GetCollisionBody() const
        {
            return collisionBody_.get();
        }

    protected:
        
        friend class EntityNotificationComponent;

        void OnComponentChanged(const Engine::Component* component) override;

    private:

        //! Updates the collision static body used for picking.
        void UpdateCollisionBody(Physics::ColliderPtr collider = nullptr);
        //! Updates the collision static body transformation.
        void UpdateCollisionBodyTransform();

        Scene::Transform& GetTransform() override;

        inline UITreeView::Entry* GetTreeViewEntry() const
        {
            return treeViewEntry_.get();
        }

        /* === Members === */

        std::unique_ptr<UITreeView::Entry>  treeViewEntry_;

        Physics::StaticBodyPtr              collisionBody_;         //!< Physics collision static body (for object selection).

        Scene::Transform                    transform_;             //!< Main transformation of the entity (will be passed to components).

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================