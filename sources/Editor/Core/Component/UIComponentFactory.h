/*
 * Editor UI component factory header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_COMPONENT_FACTORY_H__
#define __FORK_EDITOR_UI_COMPONENT_FACTORY_H__


#include "Engine/Component/ComponentFactory.h"
#include "UIComponent.h"


namespace Fork
{

namespace Editor
{


class UIPropertyGrid;

//! Base class for any customized components in the editor.
class UIComponentFactory : public Engine::ComponentFactory
{
    
    public:
        
        UIComponentFactory();

        std::unique_ptr<Engine::Component> InstantiateComponent(const Engine::Component::Types type) const override;

        /**
        Returns the component type for the specified property name prefix.
        \throws InvalidArgumentException If 'namePrefix' is unknown.
        */
        static Engine::Component::Types QueryTypeByNamePrefix(const std::string& namePrefix);
        //! Builds the property grid for all component categories.
        static void BuildPropertyGrid(UIPropertyGrid& propGrid);

    private:

        UIPropertyGrid* propGrid_ = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================