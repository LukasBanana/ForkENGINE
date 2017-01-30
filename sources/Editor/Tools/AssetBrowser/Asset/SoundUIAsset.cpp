/*
 * Editor sound UI asset file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "SoundUIAsset.h"
#include "../AssetBrowserView.h"
#include "../../../Core/Devices.h"
#include "Core/Exception/NullPointerException.h"
#include "Utility/WaveformRenderer.h"


namespace Fork
{

namespace Editor
{


using namespace Devices;

SoundUIAsset::SoundUIAsset(
    const PreviewPartition& previewPartition, const Audio::SoundSourcePtr& soundResource, const std::string& assetLabel) :
        UIAsset         { previewPartition, assetLabel },
        soundResource_  { soundResource                }
{
    ASSERT_POINTER(soundResource);

    DrawPreview();
}

UIAsset::Types SoundUIAsset::Type() const
{
    return Types::Sound;
}

UIAsset::Flags::DataType SoundUIAsset::Flags() const
{
    return 0;
}

void SoundUIAsset::DrawOverlay(const AssetBrowserView* view, const Math::Point2f& position, const Math::Size2f& size)
{
    static const float buttonSize = 0.7f;

    auto isPlaying = soundResource_->IsPlaying();

    view->DrawOverlayButton(
        (isPlaying ? view->GetStopButtonTexture() : view->GetPlayButtonTexture()),
        position + Math::Point2f(0.0f, size.height*0.5f),
        false, buttonSize
    );
}


/*
 * ======= Private: =======
 */

void SoundUIAsset::DrawPreviewContent(const Math::Size2i& previewSize)
{
    primitiveRenderer->BeginDrawing2D(false);
    primitiveRenderer->Setup2DProjection({}, previewSize.Cast<float>());

    switch (soundResource_->Type())
    {
        case Audio::SoundSource::Types::Buffer:
        {
            auto bufferSound = dynamic_cast<Audio::BufferSoundSource*>(soundResource_.get());
                
            auto size = previewSize.Cast<float>();

            //Devices::primitiveRenderer->DrawRect({ 0, 0, size.width, size.height });

            Utility::WaveformRenderer::DrawWaveform(
                Devices::primitiveRenderer.get(),
                *bufferSound->GetWaveBuffer(),
                { 0.0f, size.height/4.0f, size.width, size.height*3.0f/4.0f },
                { 0.3f, 0.3f, 1.0f },
                { 0.6f, 0.6f, 1.0f },
                0.0,
                1.0
            );
        }
        break;

        default:
        break;
    }
    
    primitiveRenderer->EndDrawing2D();
}


} // /namespace Editor

} // /namespace Fork



// ========================