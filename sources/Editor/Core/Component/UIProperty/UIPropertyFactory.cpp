/*
 * Editor UI property factory file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIPropertyFactory.h"
#include "../../UIExtensions/UIPropertyGrid.h"
#include "Core/Exception/NullPointerException.h"

#include "UIPropertyBool.h"
#include "UIPropertyInt.h"
#include "UIPropertyFloat.h"
#include "UIPropertyEnum.h"
#include "UIPropertyString.h"
#include "UIPropertyColor.h"
#include "UIPropertyVector3.h"
#include "UIPropertyVector4.h"
#include "UIPropertyAsset.h"


namespace Fork
{

namespace Editor
{


using namespace Engine;

std::unique_ptr<Component::BoolProperty> UIPropertyFactory::MakeBoolProperty(const PropertyKey& key)
{
    return MakeUIProperty<UIPropertyBool>(key);
}

std::unique_ptr<Component::IntProperty> UIPropertyFactory::MakeIntProperty(const PropertyKey& key)
{
    return MakeUIProperty<UIPropertyInt>(key);
}

std::unique_ptr<Component::FloatProperty> UIPropertyFactory::MakeFloatProperty(const PropertyKey& key)
{
    return MakeUIProperty<UIPropertyFloat>(key);
}

std::unique_ptr<Component::StringProperty> UIPropertyFactory::MakeStringProperty(const PropertyKey& key)
{
    return MakeUIProperty<UIPropertyString>(key);
}

std::unique_ptr<Component::Vector3Property> UIPropertyFactory::MakeVector3Property(const PropertyKey& key)
{
    return MakeUIProperty<UIPropertyVector3>(key);
}

std::unique_ptr<Component::Vector4Property> UIPropertyFactory::MakeVector4Property(const PropertyKey& key)
{
    return MakeUIProperty<UIPropertyVector4>(key);
}

/*std::unique_ptr<Component::ColorRGBProperty> UIPropertyFactory::MakeColorRGBProperty(const PropertyKey& key)
{
    return MakeUIProperty<UIPropertyColor>(key);
}*/

std::unique_ptr<Component::ColorRGBAProperty> UIPropertyFactory::MakeColorRGBAProperty(const PropertyKey& key)
{
    return MakeUIProperty<UIPropertyColor>(key);
}

std::unique_ptr<Engine::Component::AssetProperty> UIPropertyFactory::MakeAssetProperty(const PropertyKey& key)
{
    return MakeUIProperty<UIPropertyAsset>(key);
}


/*
 * ======= Private: =======
 */

template <class Comp> std::unique_ptr<Comp> UIPropertyFactory::MakeUIProperty(const PropertyKey& key)
{
    ASSERT_POINTER(propertyGridRef);
    return std::make_unique<Comp>(*propertyGridRef, UIPropertyGrid::GetID(namePrefix, key));
}


} // /namespace Editor

} // /namespace Fork



// ========================