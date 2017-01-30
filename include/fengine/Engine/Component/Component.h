/*
 * Component header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_COMPONENT_H__
#define __FORK_COMPONENT_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Core/StringModifier.h"
#include "IO/Core/Variant.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/Vector4.h"
#include "Video/Core/ColorRGB.h"
#include "Video/Core/ColorRGBA.h"
#include "Core/Container/Identifiable.h"
#include "Core/Container/Observable.h"

#include <string>
#include <vector>
#include <map>


namespace Fork
{

namespace IO
{

class File;

}

namespace Engine
{


class Component;
class ComponentFactory;
class GameObject;

/**
Base class for all engine components. Components can be observed by other components (when they their properties changed).
\remarks A component can be a transformation, a material, a renderer, a script or whatever.
You can also write your own custom components to extend your game objects (also called "Entities").
*/
class FORK_EXPORT Component : private Observable<Component>
{
    
    public:
        
        //! Property key type.
        typedef size_t PropertyKey;

        /* === Enumerations === */

        //! Component type enumeration.
        enum class Types
        {
            Notification = 0,   //!< \see NotificationComponent
            MetaData,           //!< \see MetaDataComponent
            Transform,          //!< \see TransformComponent
            Geometry,           //!< \see GeometryComponent
            AutoUVMap,          //!< \see AutoUVMapComponent
        };

        /* === Classes === */

        /**
        Component property class. Each property only stores a small data type.
        There are: boolean, integer, floating-point, UTF-8 string, 3D vector, 4D vector, RGB color or RGBA color.
        This is used for a unified component system, which allows easy property access and read/write operations.
        \see IO::Variant
        */
        class FORK_EXPORT Property
        {
            
            public:
                
                //! Property type enumeration.
                enum class Types
                {
                    Bool,       //!< \see BoolProperty
                    Int,        //!< \see IntProperty
                    Float,      //!< \see FloatProperty
                    String,     //!< \see StringProperty
                    Vector3,    //!< \see Vector3Property
                    Vector4,    //!< \see Vector4Property
                    ColorRGB,   //!< \see ColorRGBProperty
                    ColorRGBA,  //!< \see ColorRGBAProperty
                    Asset,      //!< \see AssetProperty
                };

                virtual ~Property();
                
                //! Returns the type of this property.
                virtual Types Type() const = 0;

                //! Writes this property data to the specified file.
                virtual void WriteToFile(IO::File& file) const = 0;
                //! Reads this property data from the specified file.
                virtual void ReadFromFile(IO::File& file) = 0;

                //! Writes this property data to the specified variant.
                virtual void WriteToVariant(IO::Variant& variant) const = 0;
                //! Reads this property data from the specified variant.
                virtual void ReadFromVariant(const IO::Variant& variant) = 0;

                //! Returns this property data as variant.
                IO::Variant Value() const;

        };
        
        //! Property array list type.
        typedef std::vector<std::unique_ptr<Property>> PropertyList;

        /*
        This is a helper macro to always declare the same functions for a component property.
        Don't extend this macro with parameters, so that the class declarations are still readable!
        */
        #define DECL_PROPERTY_INTERFACE(propType)                       \
            static const Types type = Types::propType;                  \
            Types Type() const override final;                          \
            void WriteToFile(IO::File& file) const override;            \
            void ReadFromFile(IO::File& file) override;                 \
            void WriteToVariant(IO::Variant& variant) const override;   \
            void ReadFromVariant(const IO::Variant& variant) override

        //! Boolean component property.
        class FORK_EXPORT BoolProperty : public Property
        {
            public:
                DECL_PROPERTY_INTERFACE(Bool);
                bool value = false;
        };

        //! Integer component property.
        class FORK_EXPORT IntProperty : public Property
        {
            public:
                DECL_PROPERTY_INTERFACE(Int);
                int value = 0;
        };

        //! Floating-point component property.
        class FORK_EXPORT FloatProperty : public Property
        {
            public:
                DECL_PROPERTY_INTERFACE(Float);
                float value = 0.0f;
        };

        //! UTF-8 string component property.
        class FORK_EXPORT StringProperty : public Property
        {
            public:
                DECL_PROPERTY_INTERFACE(String);
                std::string value;
        };

        //! Vector3f component property.
        class FORK_EXPORT Vector3Property : public Property
        {
            public:
                DECL_PROPERTY_INTERFACE(Vector3);
                Math::Vector3f value;
        };

        //! Vector4f component property.
        class FORK_EXPORT Vector4Property : public Property
        {
            public:
                DECL_PROPERTY_INTERFACE(Vector4);
                Math::Vector4f value;
        };

        //! ColorRGBub component property.
        class FORK_EXPORT ColorRGBProperty : public Property
        {
            public:
                DECL_PROPERTY_INTERFACE(ColorRGB);
                Video::ColorRGBub value;
        };

        //! ColorRGBAub component property.
        class FORK_EXPORT ColorRGBAProperty : public Property
        {
            public:
                DECL_PROPERTY_INTERFACE(ColorRGBA);
                Video::ColorRGBAub value;
        };

        //! Asset component property (Stores an asset ID).
        class FORK_EXPORT AssetProperty : public Property
        {
            public:
                DECL_PROPERTY_INTERFACE(Asset);
                Identifiable::IDType value = Identifiable::invalidID;
        };

        #undef DECL_PROPERTY_INTERFACE

        //! Property factory base class.
        class FORK_EXPORT PropertyFactory
        {
            
            public:
                
                virtual ~PropertyFactory();

                //! Creates a property of the specified type.
                std::unique_ptr<Property> MakeProperty(const Property::Types type, const PropertyKey& key);

                virtual std::unique_ptr< BoolProperty      > MakeBoolProperty       (const PropertyKey& key);
                virtual std::unique_ptr< IntProperty       > MakeIntProperty        (const PropertyKey& key);
                virtual std::unique_ptr< FloatProperty     > MakeFloatProperty      (const PropertyKey& key);
                virtual std::unique_ptr< StringProperty    > MakeStringProperty     (const PropertyKey& key);
                virtual std::unique_ptr< Vector3Property   > MakeVector3Property    (const PropertyKey& key);
                virtual std::unique_ptr< Vector4Property   > MakeVector4Property    (const PropertyKey& key);
                virtual std::unique_ptr< ColorRGBProperty  > MakeColorRGBProperty   (const PropertyKey& key);
                virtual std::unique_ptr< ColorRGBAProperty > MakeColorRGBAProperty  (const PropertyKey& key);
                virtual std::unique_ptr< AssetProperty     > MakeAssetProperty      (const PropertyKey& key);

        };

        /* === Functions === */

        Component(const Component&) = delete;
        Component& operator = (const Component&) = delete;

        virtual ~Component();

        /* --- Common --- */

        //! Returns the component type.
        virtual Types Type() const = 0;
        //! Returns the component ID name (e.g. "meta_data", "transform", "geometry" etc.).
        virtual std::string IDName() const = 0;

        //! Writes all properties of this component to the specified file.
        virtual void WriteToFile(IO::File& file) const;
        //! Reads all properties of this component from the specified file.
        virtual void ReadFromFile(IO::File& file);

        //! Callback when an observable allows this component to attach it to its observers.
        void RegisterObservable(Component& observable);
        //! Callback when an observable allows this component to detach it from its observers.
        void UnregisterObservable(Component& observable);

        //! Instantiates a copy of this component.
        std::unique_ptr<Component> InstantiateCopy(ComponentFactory* factory = nullptr) const;

        //! Calls "OnChanged" and notifies all component observers about a change of this component.
        void NotifyThisComponentChanged();

        /* --- Properties --- */

        /**
        Returns a raw pointer to the property with the specified key
        or null if this component has no such property.
        */
        Property* FindProperty(const PropertyKey& key) const;

        /**
        Returns a raw pointer to the property with the specified key
        or null if this component has no such property.
        \tparam Prop Specifies the property class.
        \param[in] key Specifies the property key.
        \return Pointer to the property with the specified key or null if this component does not have such property.
        */
        template <class Prop> Prop* FindProperty(const PropertyKey& key) const
        {
            auto prop = FindProperty(key);
            return (prop != nullptr && prop->Type() == Prop::type) ? static_cast<Prop*>(prop) : nullptr;
        }

        //! Returns the property list.
        inline const PropertyList& GetProperties() const
        {
            return properties_;
        }

        /* --- Write --- */

        void WritePropertyBool      (const PropertyKey& key, bool                       value);
        void WritePropertyInt       (const PropertyKey& key, int                        value);
        void WritePropertyFloat     (const PropertyKey& key, float                      value);
        void WritePropertyString    (const PropertyKey& key, const std::string&         value);
        void WritePropertyVector3   (const PropertyKey& key, const Math::Vector3f&      value);
        void WritePropertyVector4   (const PropertyKey& key, const Math::Vector4f&      value);
        void WritePropertyColorRGB  (const PropertyKey& key, const Video::ColorRGBub&   value);
        void WritePropertyColorRGBA (const PropertyKey& key, const Video::ColorRGBAub&  value);
        void WritePropertyAsset     (const PropertyKey& key, Identifiable::IDType       value);

        /* --- Read --- */

        bool                    ReadPropertyBool        (const PropertyKey& key) const;
        int                     ReadPropertyInt         (const PropertyKey& key) const;
        float                   ReadPropertyFloat       (const PropertyKey& key) const;
        std::string             ReadPropertyString      (const PropertyKey& key) const;
        Math::Vector3f          ReadPropertyVector3     (const PropertyKey& key) const;
        Math::Vector4f          ReadPropertyVector4     (const PropertyKey& key) const;
        Video::ColorRGBub       ReadPropertyColorRGB    (const PropertyKey& key) const;
        Video::ColorRGBAub      ReadPropertyColorRGBA   (const PropertyKey& key) const;
        Identifiable::IDType    ReadPropertyAsset       (const PropertyKey& key) const;

    protected:
        
        Component() = default;

        /* --- Observables --- */

        /**
        Returns the if the specified type of components is of interest as observable for this component. By default false.
        \see RegisterObservable
        \see UnregisterObservable
        */
        virtual bool IsObservableOfInterest(const Types type) const;
        //! Callback when an observable component notified all observers about a change.
        virtual void OnObservableChanged(const Component* observable);

        //! Callback when this component has changed.
        virtual void OnThisComponentChanged();

        /* --- Properties --- */

        /**
        Creates a new property with the specified key and the specified constructor arguments.
        \param[in] key Specifies the property key. This must be an index to the properties beginning with zero.
        \throws InvalidArgumentException If 'key' is invalid.
        */
        template <class Prop> Prop* CreateProperty(const PropertyKey& key, PropertyFactory* factory = nullptr)
        {
            if (!factory)
                factory = &(Component::DefaultPropertyFactory());

            auto prop = factory->MakeProperty(Prop::type, key);
            
            auto propRef = static_cast<Prop*>(prop.get());
            IncludeProperty(key, std::move(prop));

            return propRef;
        }

        //! Writes the value to the property with the specified key and returns true on success.
        template <class Prop, typename T> void WriteProperty(const PropertyKey& key, const T& value)
        {
            auto prop = FindProperty<Prop>(key);
            if (prop)
                prop->value = value;
        }

        //! Read the value from the property with the specified key and returns true on success.
        template <class Prop, typename T> void ReadProperty(const PropertyKey& key, T& value) const
        {
            auto prop = FindProperty<Prop>(key);
            if (prop)
                value = prop->value;
        }

    private:
        
        //! Returns the property factory.
        static PropertyFactory& DefaultPropertyFactory();

        void NotifyObserver(Component& observer) const override;

        /**
        Includes the specified property by adding it to the list.
        \throws InvalidArgumentException If 'key' is invalid.
        \throws NullPointerException If 'prop' is null.
        */
        void IncludeProperty(const PropertyKey& key, std::unique_ptr<Property> prop);

        PropertyList properties_;

};


} // /namespace Engine

} // /namespace Fork


#endif



// ========================