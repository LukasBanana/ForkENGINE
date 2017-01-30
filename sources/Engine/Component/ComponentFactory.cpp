/*
 * Component factory file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/Component/ComponentFactory.h"

#include "Engine/Component/MetaDataComponent.h"
#include "Engine/Component/TransformComponent.h"
#include "Engine/Component/GeometryComponent.h"
#include "Engine/Component/AutoUVMapComponent.h"


namespace Fork
{

namespace Engine
{


ComponentFactory::~ComponentFactory()
{
}

std::unique_ptr<Component> ComponentFactory::InstantiateComponent(const Component::Types type) const
{
    typedef Component::Types Ty;

    switch (type)
    {
        case Ty::MetaData:
            return std::make_unique<MetaDataComponent>();
        case Ty::Transform:
            return std::make_unique<TransformComponent>();
        case Ty::Geometry:
            return std::make_unique<GeometryComponent>();
        case Ty::AutoUVMap:
            return std::make_unique<AutoUVMapComponent>();
    }

    return nullptr;
}


} // /namespace Engine

} // /namespace Fork



// ========================