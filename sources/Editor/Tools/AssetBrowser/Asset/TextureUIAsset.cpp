/*
 * Editor texture UI asset file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TextureUIAsset.h"
#include "Video/RenderSystem/Texture/TextureMentor.h"
#include "Core/Exception/NullPointerException.h"
#include "../../../Core/Devices.h"
#include "../../../Core/UIExtensions/UIPropertyGrid.h"
#include "../../../Language/LanguageLoader.h"
#include "../../../Core/Component/UIProperty/UIPropertyString.h"
#include "../../../Core/Component/UIProperty/UIPropertyInt.h"


namespace Fork
{

namespace Editor
{


struct PropertyIDs
{
    enum : int
    {
        Width,
        Height,
        Format,
    };
};

using namespace Devices;

TextureUIAsset::TextureUIAsset(
    const PreviewPartition& previewPartition, const Video::TexturePtr& textureResource, const std::string& assetLabel) :
        UIAsset         { previewPartition, assetLabel },
        textureResource_{ textureResource              }
{
    ASSERT_POINTER(textureResource);

    DrawPreview();
}

UIAsset::Types TextureUIAsset::Type() const
{
    return Types::Texture;
}

UIAsset::Flags::DataType TextureUIAsset::Flags() const
{
    return 0;
}

void TextureUIAsset::BuildPropertyGrid(UIPropertyGrid& propGrid, const wxString& namePrefix)
{
    propGrid.AddPropertyCategory(LANG_STRING(TextureAsset), namePrefix);

    propGrid.AddIntProperty     (LANG_STRING( Width  ), UIPropertyGrid::GetID( namePrefix, PropertyIDs::Width  ))->Enable(false);
    propGrid.AddIntProperty     (LANG_STRING( Height ), UIPropertyGrid::GetID( namePrefix, PropertyIDs::Height ))->Enable(false);
    propGrid.AddStringProperty  (LANG_STRING( Format ), UIPropertyGrid::GetID( namePrefix, PropertyIDs::Format ))->Enable(false);
}


/*
 * ======= Private: =======
 */

void TextureUIAsset::DrawPreviewContent(const Math::Size2i& previewSize)
{
    primitiveRenderer->BeginDrawing2D(false);
    primitiveRenderer->Setup2DProjection({}, previewSize.Cast<float>());

    switch (textureResource_->Type())
    {
        case Video::Texture::Types::Tex2D:
        {
            auto texture2D = dynamic_cast<Video::Texture2D*>(textureResource_.get());

            auto imageSize = texture2D->GetSize().Cast<float>().ScaledSize(previewSize.Cast<float>());

            primitiveRenderer->DrawImage(
                texture2D,
                { previewSize.width/2 - imageSize.width/2,
                    previewSize.height/2 - imageSize.height/2 },
                imageSize
            );
        }
        break;

        default:
        break;
    }
    
    primitiveRenderer->EndDrawing2D();
}

void TextureUIAsset::OnShowAttributes(UIPropertyGrid& propGrid, const wxString& namePrefix)
{
    /* Query texture description */
    const auto texDesc = Video::TextureMentor::QueryTextureDesc(GetTextureResource());

    const auto format = Video::BaseTextureFormat(GetTextureResource()->GetFormat());
    std::string formatName;
    
    switch (format)
    {
        case Video::TextureFormats::R:      formatName = "R";       break;
        case Video::TextureFormats::RG:     formatName = "RG";      break;
        case Video::TextureFormats::RGB:    formatName = "RGB";     break;
        case Video::TextureFormats::RGBA:   formatName = "RGBA";    break;
    }

    /* Update property grid */
    propGrid.SetPropertyInt     ( UIPropertyGrid::GetID( namePrefix, PropertyIDs::Width  ), texDesc.size.width  );
    propGrid.SetPropertyInt     ( UIPropertyGrid::GetID( namePrefix, PropertyIDs::Height ), texDesc.size.height );
    propGrid.SetPropertyString  ( UIPropertyGrid::GetID( namePrefix, PropertyIDs::Format ), formatName          );
}


} // /namespace Editor

} // /namespace Fork



// ========================