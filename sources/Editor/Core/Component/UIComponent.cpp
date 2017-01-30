/*
 * Editor UI component file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIComponent.h"
#include "Core/StaticConfig.h"
#include "IO/Core/Log.h"
#include "../UIExtensions/UIPropertyGrid.h"
#include "../../Frame/Main/MainFrame.h"
#include "../../Frame/Main/MainFrameComponentController.h"
#include "UIProperty/UIPropertyFactory.h"


namespace Fork
{

namespace Editor
{


UIComponent::UIComponent(const std::string& namePrefix) :
    namePrefix_{ namePrefix }
{
}

void UIComponent::WriteToPropertyGrid()
{
    for (auto& prop : EngineComponent()->GetProperties())
    {
        auto uiProp = dynamic_cast<UIProperty*>(prop.get());
        if (uiProp)
            uiProp->WriteToProperty();
    }
}

void UIComponent::ReadFromProperty(const PropertyState& state)
{
    auto prop = dynamic_cast<UIProperty*>(EngineComponent()->FindProperty(state.key));
    if (prop)
    {
        /* Write property state from UI property and notify that this component changed */
        prop->ReadFromProperty(state);
        OnReadFromProperty();
    }
    #ifdef FORK_DEBUG
    else
        IO::Log::Debug("UIProperty \"" + state.name.ToStdString() + "\" not found (" __FUNCTION__ ")");
    #endif
}

void UIComponent::Show(bool show)
{
    UIComponent::PropertyGrid()->ShowProperty(namePrefix_, show);
}

UIPropertyGrid* UIComponent::PropertyGrid()
{
    /* Use the editor component view as default property grid */
    return MainFrame::Instance()->GetComponentCtrl()->GetComponentView();
}


/*
 * ======= Protected: =======
 */

Engine::Component::PropertyFactory* UIComponent::BindUIPropertyFactory(const std::string& namePrefix)
{
    static UIPropertyFactory uiPropFactory;
    
    uiPropFactory.propertyGridRef   = UIComponent::PropertyGrid();
    uiPropFactory.namePrefix        = namePrefix;

    return &uiPropFactory;
}


} // /namespace Editor

} // /namespace Fork



// ========================