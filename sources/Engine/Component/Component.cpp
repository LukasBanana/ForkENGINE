/*
 * Component file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/Component/Component.h"
#include "Engine/Component/ComponentFactory.h"
#include "Core/Exception/InvalidArgumentException.h"
#include "Core/Exception/NullPointerException.h"


namespace Fork
{

namespace Engine
{


/*
 * Property class
 */

Component::Property::~Property()
{
}

IO::Variant Component::Property::Value() const
{
    IO::Variant value;
    WriteToVariant(value);
    return value;
}


/*
 * PropertyFactory class
 */

Component::PropertyFactory::~PropertyFactory()
{
}

std::unique_ptr<Component::Property> Component::PropertyFactory::MakeProperty(const Property::Types type, const PropertyKey& key)
{
    typedef Property::Types Ty;

    switch (type)
    {
        case Ty::Bool:
            return MakeBoolProperty(key);
        case Ty::Int:
            return MakeIntProperty(key);
        case Ty::Float:
            return MakeFloatProperty(key);
        case Ty::String:
            return MakeStringProperty(key);
        case Ty::Vector3:
            return MakeVector3Property(key);
        case Ty::Vector4:
            return MakeVector4Property(key);
        case Ty::ColorRGB:
            return MakeColorRGBProperty(key);
        case Ty::ColorRGBA:
            return MakeColorRGBAProperty(key);
        case Ty::Asset:
            return MakeAssetProperty(key);
    }

    return nullptr;
}

std::unique_ptr<Component::BoolProperty> Component::PropertyFactory::MakeBoolProperty(const PropertyKey&)
{
    return std::make_unique<BoolProperty>();
}

std::unique_ptr<Component::IntProperty> Component::PropertyFactory::MakeIntProperty(const PropertyKey&)
{
    return std::make_unique<IntProperty>();
}

std::unique_ptr<Component::FloatProperty> Component::PropertyFactory::MakeFloatProperty(const PropertyKey&)
{
    return std::make_unique<FloatProperty>();
}

std::unique_ptr<Component::StringProperty> Component::PropertyFactory::MakeStringProperty(const PropertyKey&)
{
    return std::make_unique<StringProperty>();
}

std::unique_ptr<Component::Vector3Property> Component::PropertyFactory::MakeVector3Property(const PropertyKey&)
{
    return std::make_unique<Vector3Property>();
}

std::unique_ptr<Component::Vector4Property> Component::PropertyFactory::MakeVector4Property(const PropertyKey&)
{
    return std::make_unique<Vector4Property>();
}

std::unique_ptr<Component::ColorRGBProperty> Component::PropertyFactory::MakeColorRGBProperty(const PropertyKey&)
{
    return std::make_unique<ColorRGBProperty>();
}

std::unique_ptr<Component::ColorRGBAProperty> Component::PropertyFactory::MakeColorRGBAProperty(const PropertyKey&)
{
    return std::make_unique<ColorRGBAProperty>();
}

std::unique_ptr<Component::AssetProperty> Component::PropertyFactory::MakeAssetProperty(const PropertyKey&)
{
    return std::make_unique<AssetProperty>();
}


/*
 * Component class
 */

Component::~Component()
{
}

/* --- Common --- */

void Component::WriteToFile(IO::File& file) const
{
    for (const auto& prop : properties_)
        prop->WriteToFile(file);
}

void Component::ReadFromFile(IO::File& file)
{
    for (auto& prop : properties_)
        prop->ReadFromFile(file);
}

void Component::RegisterObservable(Component& observable)
{
    if (IsObservableOfInterest(observable.Type()))
        observable.AttachObserver(*this);
}

void Component::UnregisterObservable(Component& observable)
{
    if (IsObservableOfInterest(observable.Type()))
        observable.DetachObserver(*this);
}

std::unique_ptr<Component> Component::InstantiateCopy(ComponentFactory* factory) const
{
    /* Get component factory */
    static ComponentFactory defaultFactory;

    if (!factory)
        factory = &defaultFactory;

    /* Instantiate new component */
    auto copy = factory->InstantiateComponent(Type());

    /* Copy property states */
    IO::Variant value;

    for ( auto it = GetProperties().begin(), itCopy = copy->GetProperties().begin();
          it != GetProperties().end() && itCopy != copy->GetProperties().end();
          ++it, ++itCopy )
    {
        (*it)->WriteToVariant(value);
        (*itCopy)->ReadFromVariant(value);
    }

    return copy;
}

void Component::NotifyThisComponentChanged()
{
    OnThisComponentChanged();
    NotifyAllObservers();
}

/* --- Properties --- */

Component::Property* Component::FindProperty(const PropertyKey& key) const
{
    return key < properties_.size() ? properties_[key].get() : nullptr;
}

/* --- Write --- */

void Component::WritePropertyBool(const PropertyKey& key, bool value)
{
    WriteProperty<BoolProperty>(key, value);
}

void Component::WritePropertyInt(const PropertyKey& key, int value)
{
    WriteProperty<IntProperty>(key, value);
}

void Component::WritePropertyFloat(const PropertyKey& key, float value)
{
    WriteProperty<FloatProperty>(key, value);
}

void Component::WritePropertyString(const PropertyKey& key, const std::string& value)
{
    WriteProperty<StringProperty>(key, value);
}

void Component::WritePropertyVector3(const PropertyKey& key, const Math::Vector3f& value)
{
    WriteProperty<Vector3Property>(key, value);
}

void Component::WritePropertyVector4(const PropertyKey& key, const Math::Vector4f& value)
{
    WriteProperty<Vector4Property>(key, value);
}

void Component::WritePropertyColorRGB(const PropertyKey& key, const Video::ColorRGBub& value)
{
    WriteProperty<ColorRGBProperty>(key, value);
}

void Component::WritePropertyColorRGBA(const PropertyKey& key, const Video::ColorRGBAub& value)
{
    WriteProperty<ColorRGBAProperty>(key, value);
}

void Component::WritePropertyAsset(const PropertyKey& key, Identifiable::IDType value)
{
    WriteProperty<AssetProperty>(key, value);
}

/* --- Read --- */

bool Component::ReadPropertyBool(const PropertyKey& key) const
{
    bool value = false;
    ReadProperty<BoolProperty>(key, value);
    return value;
}

int Component::ReadPropertyInt(const PropertyKey& key) const
{
    int value = 0;
    ReadProperty<IntProperty>(key, value);
    return value;
}

float Component::ReadPropertyFloat(const PropertyKey& key) const
{
    float value = 0.0f;
    ReadProperty<FloatProperty>(key, value);
    return value;
}

std::string Component::ReadPropertyString(const PropertyKey& key) const
{
    std::string value;
    ReadProperty<StringProperty>(key, value);
    return value;
}

Math::Vector3f Component::ReadPropertyVector3(const PropertyKey& key) const
{
    Math::Vector3f value;
    ReadProperty<Vector3Property>(key, value);
    return value;
}

Math::Vector4f Component::ReadPropertyVector4(const PropertyKey& key) const
{
    Math::Vector4f value;
    ReadProperty<Vector4Property>(key, value);
    return value;
}

Video::ColorRGBub Component::ReadPropertyColorRGB(const PropertyKey& key) const
{
    Video::ColorRGBub value;
    ReadProperty<ColorRGBProperty>(key, value);
    return value;
}

Video::ColorRGBAub Component::ReadPropertyColorRGBA(const PropertyKey& key) const
{
    Video::ColorRGBAub value;
    ReadProperty<ColorRGBAProperty>(key, value);
    return value;
}

Identifiable::IDType Component::ReadPropertyAsset(const PropertyKey& key) const
{
    Identifiable::IDType value = 0u;
    ReadProperty<AssetProperty>(key, value);
    return value;
}


/*
 * ======= Protected: =======
 */

Component::PropertyFactory& Component::DefaultPropertyFactory()
{
    static PropertyFactory factoryInstance;
    return factoryInstance;
}

bool Component::IsObservableOfInterest(const Types type) const
{
    return false; // dummy
}

void Component::OnObservableChanged(const Component* observable)
{
    // dummy
}

void Component::OnThisComponentChanged()
{
    // dummy
}


/*
 * ======= Private: =======
 */

void Component::NotifyObserver(Component& observer) const
{
    observer.OnObservableChanged(this);
}

void Component::IncludeProperty(const PropertyKey& key, std::unique_ptr<Property> prop)
{
    /* Validate arguments */
    ASSERT_POINTER(prop);
    
    if (key != properties_.size())
    {
        throw InvalidArgumentException(
            __FUNCTION__, "key",
            "Next property key must be " + ToStr(properties_.size()) + ", but passed " + ToStr(key)
        );
    }

    /* Add property to the list and hash-map */
    properties_.push_back(std::move(prop));
}


} // /namespace Engine

} // /namespace Fork



// ========================