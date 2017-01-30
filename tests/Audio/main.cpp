
// ForkENGINE: Audio Test
// 25/11/2014

#include <fengine/import.h>
#include <fengine/helper.h>

using namespace Fork;

class AudioTest : public Engine::SimpleApp
{
    
    public:
        
        void OnInit() override;
        void OnUpdate() override;
        void OnRender() override;

    private:
        
        void DrawWaveForm(
            const Audio::WaveBuffer* waveBuffer,
            const Video::ColorRGBAf& color = { 0.3f, 0.3f, 1.0f }
        );
        
        Audio::StreamSound music;
        Audio::BufferSoundSourcePtr sound;
        Video::TextureFontPtr font;

        double waveformSeek = 0.0;
        float waveformWidth = 600.0f;

};

FORK_IMPLEMENT_APP(AudioTest);

#define TEST 6

void AudioTest::OnInit()
{
    #if TEST > 6

    InitEngine(L"ForkENGINE Audio Test", { 800, 600 }, false, 0);

    font = primitiveRenderer->CreateTextureFont({ "Calibri", 20 });

    #else

    soundSystem = engine.CreateSoundSystem(L"ForkAudioXA2");

    #endif

    const std::string path = "../../../media/Sounds/";

    {
        auto timer = engine.MakeScopedLogTimer("Duration for RIFF WAVE reading: ");
        auto waveBuffer = Audio::SoundFileHandler::ReadSound(path + "whoosh-cola.wav");

        //auto soundStream = Audio::SoundFileHandler::OpenSoundStream(path + "retro-videogame-music1.ogg");
    }

    #if TEST == 1

    for (int i = 0; i < 3; ++i)
    {
        soundSystem->PlayFromFile(path + "whoosh-cola.wav", 0, 1.0f - static_cast<float>(i)/5);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    #elif TEST == 2

    soundSystem->PlayFromFile(path + "whoosh-cola.wav", 3, 2.0f, -1.0f, 2.0f);
    soundSystem->PlayFromFile(path + "engine2.wav", 0, 1.0f, 1.0f);

    #elif TEST == 3

    auto sound = soundSystem->LoadBufferSound(path + "engine1.wav");
    sound->SetupVolume(0.3f);
    sound->Play(Audio::infiniteSoundLoop);

    system("pause");
    sound->Stop();
    system("pause");

    sound->Play(0);

    #elif TEST == 4

    //std::string filename = path + "whoosh-cola.wav";
    std::string filename = "???";

    auto sound = soundSystem->LoadBufferSound(filename);
    
    if (sound)
    {
        //#define FREQUENCY_CHANGE
        #ifdef FREQUENCY_CHANGE
        float freq = 0.5f;
        sound->SetupFrequencyRatio(freq);
        #endif

        sound->SetupVolume(0.5f);
        sound->Play(0);

        auto startTime = std::chrono::steady_clock::now();

        float x = 0.0f;

        while (sound->IsPlaying())
        {
            auto elapsed = std::chrono::steady_clock::now() - startTime;
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            x += Math::pi*0.02f;
            auto balance = std::sin(x);
            sound->SetupBalance(balance);

            #ifdef FREQUENCY_CHANGE
            sound->SetupFrequencyRatio(freq);
            freq += 0.01f;
            #endif

            IO::Log::Message(
                "playing ... " + ToStr(duration) + " ms ; balance = " + ToStr(balance, 3)
                #ifdef FREQUENCY_CHANGE
                + " ; frequency ratio = " + ToStr(freq)
                #endif
            );
        }
    }

    #elif TEST == 5

    auto sound = soundSystem->LoadBufferSound("???");

    sound->SetupVolume(0.5f);
    sound->Play();

    IO::Log::Message("q = quit, p = pause/resume");

    while (true)
    {
        char c;
        std::cin >> c;

        if (c == 'q')
            return;
        else if (c == 'p')
        {
            sound->Pause(!sound->IsPaused());
            if (sound->IsPaused())
                IO::Log::Message("Sound Paused");
            else
                IO::Log::Message("Sound Resumed");
        }
    }

    #elif TEST == 6 // STREAMING

    auto sound = soundSystem->LoadStreamSound(
        path + "retro-videogame-music1.ogg"
        //path + "Mists_of_Time-4T.ogg"
    );

    if (!sound.stream)
    {
        IO::Log::Error("Missing sound stream");
        system("pause");
        return;
    }

    Audio::PCMBuffer pcmBuffer(1024);
    int chunkCount = 0;

    sound.stream->SetupVolume(0.5f);
    sound.stream->Play();
    sound.loopCount = 1;
    //soundStream.stream->SetupFrequencyRatio(1.7f);

    //sound.reader->TimeSeek(24.0);

    bool quit = false;
    while (!quit)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
        #if 0
        
        soundSystem->UpdateStreamSound(sound);

        if (!sound.stream->IsPlaying())
            quit = true;
        
        #else

        while (sound.stream->QueueSize() < 10)
        {
            auto chunkSize = sound.reader->ReadStream(pcmBuffer);
            if (chunkSize > 0)
            {
                sound.stream->SubmitBuffer(pcmBuffer, chunkSize);
                IO::Log::Message("Stream Chunk " + ToStr(chunkCount++) + ", Queue Size = " + ToStr(sound.stream->QueueSize()));
            }
            #if 0
            else if (sound.stream->QueueSize() == 0)
            {
                quit = true;
                break;
            }
            #else
            else
            {
                sound.reader->TimeSeek(0.0);
                chunkCount = 0;
            }
            #endif
        }

        #endif

        #if 0
        static float x;
        x += 0.01f;
        auto balance = std::sin(x);
        sound.stream->SetupBalance(balance);
        #endif
    }

    #elif TEST == 7

    soundSystem->PlayFromFile(path + "retro-videogame-music3.ogg", 1, 0.5f);

    #elif TEST == 8

    music = soundSystem->LoadStreamSound(path + "retro-videogame-music3.ogg");

    music.stream->SetupVolume(0.5f);
    music.stream->SetupFrequencyRatio(0.7f);
    //music.stream->Play();
    //music.loopCount = 1;

    sound = soundSystem->LoadBufferSound(path + "whoosh-cola.wav");
    if (sound)
        sound->SetupVolume(0.5f);

    #endif

    #if TEST <= 6
    system("pause");
    #endif
}

void AudioTest::OnUpdate()
{
    soundSystem->UpdateStreamSound(music);

    if (keyboard->KeyHit(IO::KeyCodes::KeySpace) && sound)
    {
        if (sound->IsPlaying())
            sound->Pause(!sound->IsPaused());
        else
            sound->Play();
    }

    if (mouse->ButtonDown(IO::MouseKeyCodes::MouseLeft))
        waveformWidth += static_cast<float>(mouse->GetMotion().x);

    if (mouse->ButtonDown(IO::MouseKeyCodes::MouseRight))
    {
        waveformSeek += static_cast<double>(mouse->GetMotion().x) / 800.0;
        waveformSeek = Math::Clamp(waveformSeek, 0.0, 0.499);
    }
}

void AudioTest::OnRender()
{
    primitiveRenderer->BeginDrawing2D();
    {
        #if 0
        std::string time = ToStr(music.stream->SeekPosition());
        primitiveRenderer->DrawText2D(font.get(), { 15, 15 }, "Music Playback: " + time);
        #endif

        if (sound)
        {
            auto seekPos = static_cast<float>(sound->SeekPosition());

            std::string timerValue;
            {
                auto timer = engine.MakeScopedStringTimer(timerValue);
                DrawWaveForm(sound->GetWaveBuffer());
            }

            if (sound->IsPlaying())
            {
                auto posX = 15.0f + seekPos * waveformWidth;
                auto posY = 0.5f * renderContext->Resolution().height;

                primitiveRenderer->DrawLine(
                    Math::Line2f{ { posX, posY - 100.0f }, { posX, posY + 100.0f } },
                    { 1, 0, 0, 1 }
                );
            }

            primitiveRenderer->DrawText2D(font.get(), { 15, 15 }, "Sound Playback: " + ToStr(seekPos));

            primitiveRenderer->DrawText2D(
                font.get(), { 15, 35 },
                "Waveform Drawing Time: " + timerValue + "ms"
            );
        }
    }
    primitiveRenderer->EndDrawing2D();
}

void AudioTest::DrawWaveForm(const Audio::WaveBuffer* waveBuffer, const Video::ColorRGBAf& color)
{
    if (!waveBuffer)
        return;

    float posX = 15.0f;
    float posY = 0.5f * renderContext->Resolution().height;

    Math::Rectf rect{ posX, posY - 100.0f, posX + waveformWidth, posY + 100.0f };

    primitiveRenderer->DrawRect(rect);

    auto seek = std::pow(waveformSeek*2.0, 1.0/4.0)*0.5;

    Util::WaveformRenderer::DrawWaveform(
        primitiveRenderer.get(), *waveBuffer,
        rect, color, color*2.0f, seek, (1.0 - seek*2.0)
    );
}

