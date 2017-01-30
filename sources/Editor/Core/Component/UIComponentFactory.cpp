/*
 * Editor UI component factory file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIComponentFactory.h"
#include "../../Frame/Main/MainFrame.h"
#include "../../Frame/Main/MainFrameComponentController.h"
#include "Core/Exception/InvalidArgumentException.h"
#include "UIPropertyGridBuilder.h"

#include "MetaDataUIComponent.h"
#include "TransformUIComponent.h"
#include "GeometryUIComponent.h"
#include "AutoUVMapUIComponent.h"


namespace Fork
{

namespace Editor
{


UIComponentFactory::UIComponentFactory() :
    propGrid_{ MainFrame::Instance()->GetComponentCtrl()->GetComponentView() }
{
}

std::unique_ptr<Engine::Component> UIComponentFactory::InstantiateComponent(const Engine::Component::Types type) const
{
    typedef Engine::Component::Types Ty;

    switch (type)
    {
        case Ty::MetaData:
            return std::make_unique<MetaDataUIComponent>();
        case Ty::Transform:
            return std::make_unique<TransformUIComponent>();
        case Ty::Geometry:
            return std::make_unique<GeometryUIComponent>();
        case Ty::AutoUVMap:
            return std::make_unique<AutoUVMapUIComponent>();
    }

    return nullptr;
}

Engine::Component::Types UIComponentFactory::QueryTypeByNamePrefix(const std::string& namePrefix)
{
    #define CHECK_PREFIX(typeName)                                  \
        if (namePrefix == Engine::typeName##Component::GetIDName()) \
            return Engine::Component::Types::typeName

    CHECK_PREFIX( MetaData  );
    CHECK_PREFIX( Transform );
    CHECK_PREFIX( Geometry  );
    CHECK_PREFIX( AutoUVMap );

    #undef CHECK_PREFIX

    /* Unknown name prefix -> throw exception */
    throw InvalidArgumentException(__FUNCTION__, "namePrefix", "Unknown property name prefix for component type");
    return Engine::Component::Types::MetaData;
}

void UIComponentFactory::BuildPropertyGrid(UIPropertyGrid& propGrid)
{
    UIPropertyGridBuilder builder(propGrid);

    MetaDataUIComponent ::BuildPropertyGrid(builder);
    TransformUIComponent::BuildPropertyGrid(builder);
    GeometryUIComponent ::BuildPropertyGrid(builder);
    AutoUVMapUIComponent::BuildPropertyGrid(builder);
}


} // /namespace Editor

} // /namespace Fork



// ========================