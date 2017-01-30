/*
 * Editor component container file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ComponentContainer.h"
#include "UIComponentFactory.h"
#include "../UIExtensions/UIPropertyGrid.h"
#include "Core/STLHelper.h"


namespace Fork
{

namespace Editor
{


ComponentContainer::ComponentContainer(const std::shared_ptr<IdentityFactory>& idFactory) :
    GameObject{ idFactory }
{
}

void ComponentContainer::ShowUIComponents()
{
    /* Then show all categories which are related to the entity's components */
    for (auto component : GetUIComponents())
    {
        /* Apply component data to its property grid and show its category */
        component->WriteToPropertyGrid();
        component->Show();
    }
}

void ComponentContainer::OnPGChanged(const Engine::Component::Types componentType, const PropertyState& propertyState)
{
    for (auto component : GetUIComponents())
    {
        if (component->EngineComponent()->Type() == componentType)
        {
            /* Apply property state changes */
            component->ReadFromProperty(propertyState);
            break;
        }
    }
}

void ComponentContainer::Select()
{
    AttachObserver();
    OnSelect();
}

void ComponentContainer::Deselect()
{
    OnDeselect();
    DetachObserver();
}


/*
 * ======= Protected: =======
 */

Engine::ComponentFactory* ComponentContainer::GetComponentFactory() const
{
    static UIComponentFactory factoryUI;
    return &factoryUI;
}

void ComponentContainer::OnRegisterComponent(Engine::Component* comp)
{
    auto compUI = dynamic_cast<UIComponent*>(comp);
    if (compUI)
        componentsUI_.push_back(compUI);
}

void ComponentContainer::OnUnregisterComponent(Engine::Component* comp)
{
    auto compUI = dynamic_cast<UIComponent*>(comp);
    if (compUI)
        RemoveFromList(componentsUI_, compUI);
}

void ComponentContainer::OnSelect()
{
    // dummy
}

void ComponentContainer::OnDeselect()
{
    // dummy
}


} // /namespace Editor

} // /namespace Fork



// ========================