/*
 * Editor sound UI asset header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_SOUND_UI_ASSET_H__
#define __FORK_EDITOR_SOUND_UI_ASSET_H__


#include "UIAsset.h"
#include "Audio/SoundSystem/SoundSource.h"


namespace Fork
{

namespace Editor
{


//! Editor sound asset class.
class SoundUIAsset : public UIAsset
{
    
    public:
        
        //! \throws NullPointerException If 'soundResource' is null.
        SoundUIAsset(const PreviewPartition& previewPartition, const Audio::SoundSourcePtr& soundResource, const std::string& assetLabel = "");

        //! Returns Types::Sound.
        Types Type() const override;
        Flags::DataType Flags() const override;

        void DrawOverlay(const AssetBrowserView* view, const Math::Point2f& position, const Math::Size2f& size) override;

        inline const Audio::SoundSourcePtr& GetSharedSoundResource() const
        {
            return soundResource_;
        }
        inline Audio::SoundSource* GetSoundResource() const
        {
            return soundResource_.get();
        }

    private:

        void DrawPreviewContent(const Math::Size2i& previewSize) override;

        Audio::SoundSourcePtr soundResource_; //!< The sound asset resource.

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================