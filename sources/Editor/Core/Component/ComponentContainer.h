/*
 * Editor component container header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_COMPONENT_CONTAINER_H__
#define __FORK_EDITOR_COMPONENT_CONTAINER_H__


#include "Engine/Game/GameObject.h"
#include "UIPropertyGridObserver.h"


namespace Fork
{

namespace Editor
{


//! Base class for all compontent containers.
class ComponentContainer : public Engine::GameObject, public Editor::UIPropertyGridObserver
{
    
    public:

        //! Shows all UI components of this object in the associated property grid.
        void ShowUIComponents();

        void OnPGChanged(const Engine::Component::Types componentType, const PropertyState& propertyState) override;

    protected:
        
        friend class UIComponent;

        ComponentContainer(const std::shared_ptr<IdentityFactory>& idFactory);

        /* --- Components --- */

        Engine::ComponentFactory* GetComponentFactory() const override;

        //! Adds the UI component.
        void OnRegisterComponent(Engine::Component* comp) override;
        //! Removes the UI component.
        void OnUnregisterComponent(Engine::Component* comp) override;

        //! Returns the list of all UI components.
        inline const std::vector<UIComponent*>& GetUIComponents() const
        {
            return componentsUI_;
        }

        /* --- Selection --- */

        friend class AssetBrowserModel;
        friend class EntitySelector;

        //! Selects this object and attaches the observer to the associated property grids.
        void Select();
        //! Deselects this object and detaches the observer from the associated property grids.
        void Deselect();

        //! Callback when this object is selected.
        virtual void OnSelect();
        //! Callback when this object is deselected.
        virtual void OnDeselect();

    private:
        
        //! References to all UI components for faster access.
        std::vector<UIComponent*> componentsUI_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================