
namespace Fork
{

namespace Scene
{

class MeshSkeleton
{
	class Joint
	{
		Joint* parent;
		list<unique_ptr<Joint>> children;
		Transform3Df transform;
	};
	
	void UpdateVertexWeights();
	
	GeometryPtr geometry;
	Texture2DPtr vertexWeightsMap;
};

}

namespace Anim
{

class Playback
{
	enum class Modes
	{
		OneShot,
		OneLoop,
		Loop,
		PingPong,
		PingPongLoop,
	};
	
	unsigned int firstFrame = 0, lastFrame = 0; // [0 .. +inf)
	unsigned int frame = 0;
	float interpolator = 0.0; // [0.0 .. 1.0]
	float speed = 1.0; // [-inf .. +inf]
	Modes mode = Modes::OneShot;
};

class KeyframeSequence
{
	struct PositionKeyframe
	{
		Vector3f position;
		unsigned int frame;
	};
	struct RotationKeyframe
	{
		Quaternionf rotation;
		unsigned int frame;
	};
	struct ScaleKeyframe
	{
		Vector3f scale;
		unsigned int frame;
	};
	list<PositionKeyframe> positionKeyframes;
	list<RotationKeyframe> rotationKeyframes;
	list<ScaleKeyframe> scaleKeyframes;
	
	void BuildKeyframes();
	list<Transform3Df> keyframes;
};

class Animation
{
	class EventReceiver
	{
		void OnNextFrame(Playback& playback); // when next frame is queried for the playback
		void OnFinish(Playback& playback); // when playback has finished
	};
	
	void Play(const Playback& playback);
	void Stop();
	void Pause(bool paused = true);
	
	void Update();
};

class SkeletalAnimation : public Animation
{
	class JointGroup
	{
		string name;
		list<MeshSkeleton::JointPtr> joints;
	};
	MeshSkeletonPtr skeleton;
	list<JointGroup> jointGroups;
	
	JointGroup* FindJointGroup(string name);
	
	void Update();
};

class NodeAnimation : public Animation
{
	DynamicSceneNodePtr sceneNode;
	
	void Update();
};

}

}

