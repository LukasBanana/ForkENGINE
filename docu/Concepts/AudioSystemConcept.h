
#include <fengine/import.h>

using namespace Fork;

namespace Fork
{

namespace Audio
{


class SoundSource
{
	
	public:
		
		class Buffer;
		
		virtual void Play2D(float volume = 1.0f, float balance = 0.0f) = 0;
		virtual void Play3D(const Math::Point3f& position, float volume = 1.0f) = 0;
		virtual void Pause(bool isPaused = true) = 0;
		virtual void Stop() = 0;
		
};

class SoundEmitter
{
	
	public:
		
		virtual void Play() = 0;
		
		void Pause(bool isPaused = true);
		void Stop();
		
		virtual void SetupVolume(float volume) = 0;
		
		inline float GetVolume() const
		{
			return volume_;
		}
		
		SoundSourcePtr soundSource;
		
	private:
		
		float volume_ = 1.0f;
		
};

class SoundEmitter2D : public SoundEmitter
{
	
	public:
		
		virtual void SetupBalance(float balance) = 0;
		
		inline float GetBalance() const
		{
			return balance_;
		}
		
	private:
		
		float balance_ = 0.0f;
		
};

class SoundEmitter3D : public SoundEmitter
{
	
	public:
		
		virtual void SetupPosition(const Math::Point3f& position) = 0;
		
		virtual void SetupRollOff(float rollOff) = 0;
		
		inline const Math::Point3f& GetPosition() const
		{
			return position_;
		}
		
		inline float GetRollOff() const
		{
			return rollOff_;
		}
		
	private:
		
		Math::Point3f	position_;
		float			rollOff_	= 0.0f;
		
};


} // /namespace Audio

} // /namespace Fork

int main()
{
	auto soundSystem = Audio::SoundSystem::Create(L"ForkAudioXA2");
	
	auto soundSource = soundSystem->LoadSound("MySound.wav");
	
	soundSource->Play(0.5f);
	
	auto soundEmitter = soundSystem->Create2DEmitter(soundSource);
	//soundEmitter->soundSource
	soundEmitter->SetupVolume(0.5f);
	soundEmitter->Play();
	
	auto soundEmitter3D = soundSystem->Create3DEmitter(soundSource);
	audioSceneNode->soundEmitter = soundEmitter3D;
	
	soundSystem->PlayFromFile("MySound.wav", 0.5f);
	
	return 0;
}

