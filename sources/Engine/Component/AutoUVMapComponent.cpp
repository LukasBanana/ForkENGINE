/*
 * Auto UV map component file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/Component/AutoUVMapComponent.h"
#include "Engine/Game/GameObject.h"
#include "Scene/Geometry/Node/Geometry.h"


namespace Fork
{

namespace Engine
{


AutoUVMapComponent::AutoUVMapComponent(PropertyFactory* propertyFactory)
{
    CreateProperty< BoolProperty  >( PropertyIDs::Update,   propertyFactory );
    CreateProperty< BoolProperty  >( PropertyIDs::Global,   propertyFactory );
    CreateProperty< FloatProperty >( PropertyIDs::OffsetU,  propertyFactory );
    CreateProperty< FloatProperty >( PropertyIDs::OffsetV,  propertyFactory );
    CreateProperty< FloatProperty >( PropertyIDs::ScaleU,   propertyFactory )->value = 1.0f;
    CreateProperty< FloatProperty >( PropertyIDs::ScaleV,   propertyFactory )->value = 1.0f;
}

Component::Types AutoUVMapComponent::Type() const
{
    return Types::AutoUVMap;
}

std::string AutoUVMapComponent::IDName() const
{
    return AutoUVMapComponent::GetIDName();
}

std::string AutoUVMapComponent::GetIDName()
{
    return "auto_uv_map";
}


/*
 * ======= Private: =======
 */

bool AutoUVMapComponent::IsObservableOfInterest(const Types type) const
{
    return type == Types::Transform;
}

void AutoUVMapComponent::OnObservableChanged(const Component* observable)
{
    switch (observable->Type())
    {
        case Types::Transform:
        {
            /* Notify about a change when the transformation has changed and the UV mapping is global */
            if (ReadPropertyBool(PropertyIDs::Update) && ReadPropertyBool(PropertyIDs::Global))
                NotifyThisComponentChanged();
        }
        break;
    }
}


} // /namespace Engine

} // /namespace Fork



// ========================