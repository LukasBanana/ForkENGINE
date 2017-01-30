/*
 * Geometry component file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/Component/GeometryComponent.h"
#include "Engine/Game/GameObject.h"
#include "Core/Exception/NullPointerException.h"

#include "Engine/Component/TransformComponent.h"
#include "Engine/Component/AutoUVMapComponent.h"


namespace Fork
{

namespace Engine
{


GeometryComponent::GeometryComponent(PropertyFactory* propertyFactory) :
    sceneNode_{ std::make_shared<Scene::GeometryNode>() }
{
    CreateProperty< AssetProperty >( PropertyIDs::Asset, propertyFactory );
}

Component::Types GeometryComponent::Type() const
{
    return Types::Geometry;
}

std::string GeometryComponent::IDName() const
{
    return GeometryComponent::GetIDName();
}

std::string GeometryComponent::GetIDName()
{
    return "geometry";
}

void GeometryComponent::SetupGeometry(const Scene::GeometryPtr& geometry)
{
    //WritePropertyAsset ...
    sceneNode_->geometry = geometry;
    NotifyThisComponentChanged();
}


/*
 * ======= Private: =======
 */

bool GeometryComponent::IsObservableOfInterest(const Types type) const
{
    return type == Types::Transform || type == Types::AutoUVMap;
}

void GeometryComponent::OnObservableChanged(const Component* observable)
{
    switch (observable->Type())
    {
        case Types::Transform:
        {
            auto component = dynamic_cast<const TransformComponent*>(observable);
            sceneNode_->transform = component->GetTransform();
        }
        break;

        case Types::AutoUVMap:
        {
            auto component = dynamic_cast<const AutoUVMapComponent*>(observable);
            auto isUpdate = component->ReadPropertyBool(AutoUVMapComponent::PropertyIDs::Update);

            if (isUpdate)
            {
                if (sceneNode_->geometry)
                {
                    auto isGlobal   = component->ReadPropertyBool   ( AutoUVMapComponent::PropertyIDs::Global  );
                    auto offsetU    = component->ReadPropertyFloat  ( AutoUVMapComponent::PropertyIDs::OffsetU );
                    auto offsetV    = component->ReadPropertyFloat  ( AutoUVMapComponent::PropertyIDs::OffsetV );
                    auto scaleU     = component->ReadPropertyFloat  ( AutoUVMapComponent::PropertyIDs::ScaleU  );
                    auto scaleV     = component->ReadPropertyFloat  ( AutoUVMapComponent::PropertyIDs::ScaleV  );

                    /* Modify geometry graph with auto UV map modifier */
                    Scene::GeometryAutoUVMapModifier modifier;

                    modifier.uvMapOffset    = { offsetU, offsetV };
                    modifier.uvMapScale     = { scaleU, scaleV };

                    if (isGlobal)
                        modifier.uvMapTransform = sceneNode_->transform.GetMatrix();

                    modifier.ModifyGeometryGraph(sceneNode_->geometry.get());
                }
            }
        }
        break;
    }
}


} // /namespace Engine

} // /namespace Fork



// ========================