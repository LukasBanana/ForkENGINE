/*
 * Game object header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GAMEOBJECT_H__
#define __FORK_GAMEOBJECT_H__


#include "Core/Export.h"
#include "Core/Container/Identifiable.h"
#include "IO/FileSystem/File.h"
#include "Engine/Component/Component.h"
#include "Scene/Node/DynamicSceneNode.h"

#include <vector>
#include <memory>
#include <functional>


namespace Fork
{

namespace Engine
{


/**
Base class for all entities in the editor view.
\todo RENAME to "Entity"
*/
class FORK_EXPORT GameObject : private Identifiable
{

    private:

        typedef std::vector<std::unique_ptr<Component>> ComponentList;

    public:
        
        GameObject(const GameObject&) = delete;
        GameObject& operator = (const GameObject&) = delete;

        GameObject(const std::shared_ptr<IdentityFactory>& idFactory);
        virtual ~GameObject();

        /* --- Common --- */

        //! Returns the ID number of this entity.
        inline Identifiable::IDType GetID() const
        {
            return Identifiable::GetID();
        }

        //! Instantiates a copy of this game object. This will also copy all components.
        std::unique_ptr<GameObject> InstantiateCopy(
            const std::shared_ptr<IdentityFactory>& idFactory, ComponentFactory* componentFactory = nullptr
        ) const;

        /* --- I/O system --- */

        //! Loads the game object content from the specified file (*.forkscene files).
        virtual bool LoadContent(IO::File& file);
        //! Saves the game object content to the specified file (*.forkscene files).
        virtual bool SaveContent(IO::File& file);

        //! Loads the game object state from the specified file (*.forkstate files).
        virtual bool LoadState(IO::File& file);
        //! Saves the game object state to the specified file (*.forkstate files).
        virtual bool SaveState(IO::File& file);

        /* --- Components --- */

        /**
        Creates a new component of the specified type for this game object.
        \param[in] type Specifies the component type. This type must be unique within all components of this game object.
        \return Pointer to the new component.
        \remarks All components within a game object must be unique, i.e. a game object must not have several components of the same type!
        \throws ComponentException If adding the specified component failed.
        */
        Component* CreateComponent(const Component::Types type);
        //! Deletes the component of the specified type and removes it from this game object.
        void DeleteComponent(const Component::Types type);

        //! Returns the component with the specified type or null if there is no such component.
        Component* FindComponent(const Component::Types type) const;

        //! Returns the component list.
        inline const ComponentList& GetComponents() const
        {
            return components_;
        }

        /**
        Callback signature for the "ForEachComponentWithSceneNode" function.
        \param[in] component Specifies the current component.
        \param[in] sceneNode Specifies the scene node which belongs to the component.
        */
        typedef std::function<void (Component* component, const Scene::DynamicSceneNodePtr& sceneNode)> ComponentWithSceneNodeProc;

        //! Iterates over all components and calls the callback for each component with a dynamic scene node.
        void ForEachComponentWithSceneNode(const ComponentWithSceneNodeProc& callback);

    protected:
        
        friend class NotificationComponent;

        //! Returns the component factory. If this is null, the default factory is used. By default null.
        virtual ComponentFactory* GetComponentFactory() const;

        /**
        Callback when the specified component is registered.
        \remarks At that point, 'comp' has not been added to the component list yet!
        \see GetComponents
        */
        virtual void OnRegisterComponent(Component* comp);
        /**
        Callback when the specified component is unregistered.
        \remarks At that point, 'comp' has already been removed from the component list!
        \see GetComponents
        */
        virtual void OnUnregisterComponent(Component* comp);

        /**
        Callback when a component has changed. This will only be called if this game object has the "NotificationComponent".
        This exceptional component notifies its owner (this game object) about any change of all the other components.
        \see NotificationComponent
        */
        virtual void OnComponentChanged(const Component* comp);

    private:
        
        //! Registers the specified component. 
        void RegisterComponent(Component* comp);
        //! Unregisters the specified component.
        void UnregisterComponent(Component* comp);

        //! Game object component list.
        ComponentList components_;

};


} // /namespace Engine

} // /namespace Fork


#endif



// ========================