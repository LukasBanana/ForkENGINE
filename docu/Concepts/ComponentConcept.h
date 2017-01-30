
namespace Fork
{

namespace Engine
{


// Value variant class.
class Variant
{
	
	public:
		
		Variant() = default;
		Variant(bool value);
		Variant(int value);
		Variant(float value);
		Variant(const std::string& value);
		Variant(const Math::Vector4f& value);
		Variant(const Video::ColorRGBAf& value);
		
		inline bool GetBool() const;
		//...
		
		inline operator bool () const;
		//...
		
	private:
		
		bool				valueBool_	= false;
		int					valueInt_	= 0;
		float				valueFloat_	= 0.0f;
		std::string			valueString_;
		Math::Vector4f		valueVector_;
		Video::ColorRGBAf	valueColor_;
		
};

class Component
{
	
	public:
		
		class Property
		{
			
			public:
				
				virtual ~Property();
				
				virtual void WriteToFile(IO::File& file) = 0;
				virtual void ReadFromFile(IO::File& file) = 0;
				
				virtual void WriteToVariant(Variant& value) = 0;
				virtual void ReadFromVariant(const Variant& value) = 0;
				
				Variant Value() const
				{
					Variant value;
					WriteToVariant(value);
					return value;
				}
				
		};
		
		#define DECL_PROPERTY_INTERFACE					\
			void WriteToFile(IO::File& file);			\
			void ReadFromFile(IO::File& file);			\
			void WriteToVariant(Variant& value);		\
			void ReadFromVariant(const Variant& value);
		
		//! Boolean component property.
		class BoolProperty : public Property
		{
			public:
				DECL_PROPERTY_INTERFACE
				virtual ~BoolProperty();
				bool value = false;
		};
		
		class IntProperty : public Property { /* ... */ };
		class FloatProperty : public Property { /* ... */ };
		class ColorRGBProperty : public Property { /* ... */ };
		class ColorRGBAProperty : public Property { /* ... */ };
		class Vector3Property : public Property { /* ... */ };
		class Vector4Property : public Property { /* ... */ };
		
		#define DECL_PROPERTY_INTERFACE
		
		struct PropertyKey
		{
			PropertyKey(const std::string& str) :
				key(str)
			{
			}
			PropertyKey(int keyID) :
				key(ToStr(keyID))
			{
			}
			
			inline operator std::string () const
			{
				return key;
			}
			
			std::string key;
		};
		
		virtual ~Component();
		
		virtual void WriteToFile(IO::File& file);
		virtual void ReadFromFile(IO::File& file);
		
		Property* FindProperty(const PropertyKey& key) const;
		
		inline const std::vector<std::unique_ptr<Property>>& GetProperties() const
		{
			return properties_;
		}
		
		/* --- Write --- */
		
		void WritePropertyBool(const PropertyKey& key, bool value);
		//...
		
		/* --- Read --- */
		
		bool ReadPropertyBool(const PropertyKey& key) const;
		//...
		
	protected:
		
		Component() = default;
		
		template <class Prop, typename... Args> void CreateProperty(const PropertyKey& key, Args... args)
		{
			auto prop = std::make_unique<Prop>(args);
			AddProperty(key, prop);
		}
		
		template <class Prop, typename T> bool WriteProperty(const PropertyKey& key, const T& value)
		{
			auto prop = dynamic_cast<Prop*>(FindProperty(key));
			if (prop)
			{
				prop->value = value;
				return true;
			}
			return false;
		}
		
		template <class Prop, typename T> bool ReadProperty(const PropertyKey& key, T& value) const
		{
			auto prop = dynamic_cast<Prop*>(FindProperty(key));
			if (prop)
			{
				value = prop->value;
				return true;
			}
			return false;
		}
		
	private:
		
		void AddProperty(const PropertyKey& key, std::unique_ptr<Property>& prop)
		{
			auto propRef = prop.get();
			properties_.push_back(std::move(prop));
			propertyKeys_[key] = propRef;
		}
		
		std::vector<std::unique_ptr<Property>> properties_;
		std::map<std::string, Property*> propertyKeys_;
		
};


} // /namespace Engine

} // /namespace Fork
