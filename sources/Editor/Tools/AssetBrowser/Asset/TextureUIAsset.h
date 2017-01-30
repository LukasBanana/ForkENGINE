/*
 * Editor texture UI asset header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_TEXTURE_UI_ASSET_H__
#define __FORK_EDITOR_TEXTURE_UI_ASSET_H__


#include "UIAsset.h"
#include "Video/RenderSystem/Texture/Texture.h"


namespace Fork
{

namespace Editor
{


//! Editor texture asset class.
class TextureUIAsset : public UIAsset
{
    
    public:
        
        //! \throws NullPointerException If 'textureResource' is null.
        TextureUIAsset(const PreviewPartition& previewPartition, const Video::TexturePtr& textureResource, const std::string& assetLabel = "");

        //! Returns Types::Texture.
        Types Type() const override;
        Flags::DataType Flags() const override;

        inline const Video::TexturePtr& GetSharedTextureResource() const
        {
            return textureResource_;
        }
        inline Video::Texture* GetTextureResource() const
        {
            return textureResource_.get();
        }

        static void BuildPropertyGrid(UIPropertyGrid& propGrid, const wxString& namePrefix);

    private:

        void DrawPreviewContent(const Math::Size2i& previewSize) override;

        void OnShowAttributes(UIPropertyGrid& propGrid, const wxString& namePrefix) override;

        Video::TexturePtr textureResource_; //!< The texture asset resource.

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================