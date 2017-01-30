
namespace Fork
{

namespace Video
{


class GeometryRenderer
{
	
	public:
		
		void Render(Geometry* geometry);
		
		Video::ShaderCompoundPtr shader;
		
};


}

namespace Engine
{


class Component
{
	
	public:
		
		virtual std::string Name() const = 0;
		
		virtual void WriteToEntity(Entity* entity);
		virtual void ReadFromEntity(const Entity* entity);
		
};

class TransformComponent : public Component
{
	
	public:
		
		std::string Name() const
		{
			return "Transform";
		}
		
		Scene::Transform transform;
		
};

class RigidBodyComponent : public Component
{
	
	public:
		
		std::string Name() const
		{
			return "Rigid Body";
		}
		
		Physics::RigidBodyPtr rigidBody;
		
};

class GeometryRendererComponent : public Component
{
	
	public:
		
		std::string Name() const
		{
			return "Geometry Renderer";
		}
		
		Video::GeometryRendererPtr renderer;
		
};

class Entity
{
	
	public:
		
		void AddComponent(const ComponentPtr& component);
		
		template <class T, typename... Args> std::shared_ptr<T> AddComponent(Args... args)
		{
			auto component = std::make_shared<T>(args);
			AddComponent(component);
			return component;
		}
		
		Scene::Transform*			transform	= nullptr;
		Physics::RigidBody*			rigidBody	= nullptr;
		Physics::StaticBody*		staticBody	= nullptr;
		Scene::CameraNode*			camera		= nullptr;
		Scene::LightSource*			light		= nullptr;
		Video::Geometry*			geometry	= nullptr;
		Video::GeometryRenderer*	renderer	= nullptr;
		
};


} // /namespace Engine

} // /namespace Fork
