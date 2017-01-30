/*
 * Texture manager file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/Texture/TextureManager.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Video/RenderSystem/RenderContext.h"
#include "Video/FileHandler/ImageFileHandler.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/STLHelper.h"
#include "IO/Core/Log.h"
#include "../RenderSysCtx.h"

#include <algorithm>


namespace Fork
{

namespace Video
{


/* === Internal functions === */

static void FilterMaxImageSize(const Math::Size3st& imageSize, Math::Size3st& maxImageSize, bool& mustResize)
{
    if (maxImageSize.width > 0 && maxImageSize.height > 0 && imageSize != maxImageSize)
    {
        IO::Log::Warning("Inconsistent image sizes for texture array or cube (Resize will slow down performance)");
        mustResize = true;
    }

    maxImageSize.width  = std::max( maxImageSize.width,  imageSize.width  );
    maxImageSize.height = std::max( maxImageSize.height, imageSize.height );
    maxImageSize.depth  = std::max( maxImageSize.depth,  imageSize.depth  );
};

static void FilterCommonColorFormat(const ImageColorFormats colorFormat, ImageColorFormats& commonColorFormat)
{
    if (commonColorFormat < colorFormat)
        commonColorFormat = colorFormat;
};

template <class ImageContainer> void ResizeAllImages(ImageContainer& images, const Math::Size3st& imageSize)
{
    /* Resize all images to a consistent size */
    for (auto& image : images)
    {
        if (image)
            image->Resize(imageSize);
    }
}


/* --- Texture2D Array --- */

Texture2DPtr TextureManager::CreateTexture2DFromFile(const std::string& filename, bool hasHDR)
{
    /* Get active render system and render context */
    auto renderSystem = RenderSys();
    auto renderContext = RenderCtx();

    /* Print information */
    IO::Log::Message("Load texture: \"" + filename + "\"");
    IO::Log::ScopedIndent indent;

    /* Create texture */
    Video::Texture2DPtr texture;

    if (hasHDR)
    {
        /* Read HDR image from file */
        auto image = ImageFileHandler::ReadImageHDR(filename);

        if (image)
        {
            /* Adjust image format alignment (RGB -> RGBA, BGR -> BGRA) */
            image->AdjustFormatAlignment();

            /* Create and initialize texture */
            texture = renderSystem->CreateTexture2D();

            //!TODO! -> WriteTexture(texture.get(), *image); !!!!!!
            renderSystem->WriteTexture(
                texture.get(),
                ChooseTextureFormat(image->GetFormat(), Video::RendererDataTypes::Float),
                image->GetSize().Sz2().Cast<int>(), 0,
                image->GetFormat(), Video::RendererDataTypes::Float, image->RawBuffer()
            );
        }
    }
    else
    {
        /* Read image from file */
        auto image = ImageFileHandler::ReadImage(filename);

        if (image)
        {
            /* Adjust image format alignment (RGB -> RGBA, BGR -> BGRA) */
            image->AdjustFormatAlignment();

            /* Create and initialize texture */
            texture = renderSystem->CreateTexture2D();
            renderSystem->WriteTexture(texture.get(), *image);
        }
    }

    if (texture)
    {
        texture->metaData.name = filename;

        renderContext->GenerateMIPMaps(texture.get());

        /* Notify all event handlers about the created texture */
        for (const auto& eventHandler : eventHandlers_)
            eventHandler->OnTexture2DCreated(texture);

        return AddTexture(texture);
    }

    return nullptr;
}

/* --- Texture2D Array --- */

Texture2DPtr TextureManager::CreateTexture2DArrayFromFiles(const std::vector<std::string>& filenames, bool hasHDR)
{
    /* Get active render system */
    auto renderSystem = RenderSys();
    auto renderContext = RenderCtx();

    /* Temporary memory */
    Math::Size3st maxImageSize;
    bool mustResize = false;
    ImageColorFormats commonColorFormat = ImageColorFormats::Gray;

    /* Create texture */
    IO::Log::Message("Create texture 2D array");
    IO::Log::ScopedIndent indent;

    auto texture = renderSystem->CreateTexture2D();

    if (hasHDR)
    {
        /* Read all HDR images */
        std::vector<ImageFloatPtr> images;

        for (const auto& filename : filenames)
        {
            IO::Log::Message("Load texture: \"" + filename + "\"");
            IO::Log::ScopedIndent indent;

            /* Read HDR image from file */
            auto image = ImageFileHandler::ReadImageHDR(filename);
            images.push_back(image);

            if (image)
            {
                FilterMaxImageSize(image->GetSize(), maxImageSize, mustResize);
                FilterCommonColorFormat(image->GetFormat(), commonColorFormat);
            }
        }

        if (mustResize)
            ResizeAllImages(images, maxImageSize);

        /* Setup texture buffer */
        const auto arraySize = static_cast<unsigned int>(images.size());
        const auto texSize = maxImageSize.Sz2().Cast<int>();

        renderSystem->WriteTexture(
            texture.get(),
            ChooseTextureFormat(commonColorFormat, RendererDataTypes::Float),
            texSize,
            arraySize
        );

        /* Initialize texture with image data */
        unsigned int arrayIndex = 0;

        for (const auto& image : images)
        {
            if (image)
            {
                /* Adjust format alignment (for specific renderer requirements) */
                //if (renderSystem->RequireColorFormatAlignment())
                image->AdjustFormatAlignment();

                /* Write image data to texture */
                renderSystem->WriteSubTexture(
                    texture.get(), { 0, 0 }, texSize, arrayIndex,
                    image->GetFormat(), RendererDataTypes::Float, image->RawBuffer()
                );
            }
            ++arrayIndex;
        }
    }
    else
    {
        /* Read all images */
        std::vector<ImageUBytePtr> images;

        for (const auto& filename : filenames)
        {
            IO::Log::Message("Load texture: \"" + filename + "\"");
            IO::Log::ScopedIndent indent;

            /* Read image from file */
            auto image = ImageFileHandler::ReadImage(filename);
            images.push_back(image);

            if (image)
            {
                FilterMaxImageSize(image->GetSize(), maxImageSize, mustResize);
                FilterCommonColorFormat(image->GetFormat(), commonColorFormat);
            }
        }

        if (mustResize)
            ResizeAllImages(images, maxImageSize);

        /* Setup texture buffer */
        const auto arraySize = static_cast<unsigned int>(images.size());
        const auto texSize = maxImageSize.Sz2().Cast<int>();

        renderSystem->WriteTexture(
            texture.get(),
            ChooseTextureFormat(commonColorFormat, RendererDataTypes::UByte),
            texSize,
            arraySize
        );

        /* Initialize texture with image data */
        unsigned int arrayIndex = 0;

        for (const auto& image : images)
        {
            if (image)
            {
                /* Adjust format alignment (for specific renderer requirements) */
                //if (renderSystem->RequireColorFormatAlignment())
                image->AdjustFormatAlignment();

                /* Write image data to texture */
                renderSystem->WriteSubTexture(
                    texture.get(), { 0, 0 }, texSize, arrayIndex,
                    image->GetFormat(), RendererDataTypes::UByte, image->RawBuffer()
                );
            }
            ++arrayIndex;
        }
    }

    renderContext->GenerateMIPMaps(texture.get());

    return AddTexture(texture);
}

/* --- TextureCube --- */

TextureCubePtr TextureManager::CreateTextureCubeFromFiles(const Math::Cuboid<std::string>& filenames, bool hasHDR)
{
    /* Get active render system */
    auto renderSystem = RenderSys();
    auto renderContext = RenderCtx();

    /* Temporary memory */
    Math::Size3st maxImageSize;
    bool mustResize = false;
    ImageColorFormats commonColorFormat = ImageColorFormats::Gray;

    typedef Video::TextureCube::Faces CubeFaces;
    static const CubeFaces cubeFaces[]
    {
        CubeFaces::XNegative, CubeFaces::YPositive, CubeFaces::ZNegative,
        CubeFaces::XPositive, CubeFaces::YNegative, CubeFaces::ZPositive
    };

    /* Create texture */
    IO::Log::Message("Create texture cube");
    IO::Log::ScopedIndent indent;

    auto texture = renderSystem->CreateTextureCube();

    if (hasHDR)
    {
        /* Read all HDR images */
        std::vector<ImageFloatPtr> images;

        for (size_t i = 0; i < 6; ++i)
        {
            const auto& filename = filenames[i];

            IO::Log::Message("Load texture: \"" + filename + "\"");
            IO::Log::ScopedIndent indent;

            /* Read HDR image from file */
            auto image = ImageFileHandler::ReadImageHDR(filename);
            images.push_back(image);

            if (image)
            {
                FilterMaxImageSize(image->GetSize(), maxImageSize, mustResize);
                FilterCommonColorFormat(image->GetFormat(), commonColorFormat);
            }
        }

        if (mustResize)
            ResizeAllImages(images, maxImageSize);

        /* Setup texture buffer */
        const auto texSize = maxImageSize.Sz2().Cast<int>();

        renderSystem->WriteTexture(
            texture.get(),
            ChooseTextureFormat(commonColorFormat, RendererDataTypes::Float),
            texSize
        );

        /* Initialize texture with image data */
        size_t faceIndex = 0;

        for (const auto& image : images)
        {
            if (image)
            {
                /* Adjust format alignment (for specific renderer requirements) */
                //if (renderSystem->RequireColorFormatAlignment())
                image->AdjustFormatAlignment();

                /* Write image data to texture */
                renderSystem->WriteSubTexture(
                    texture.get(), { 0, 0 }, texSize, cubeFaces[faceIndex], 0,
                    image->GetFormat(), RendererDataTypes::Float, image->RawBuffer()
                );
            }
            ++faceIndex;
        }
    }
    else
    {
        /* Read all images */
        std::vector<ImageUBytePtr> images;

        for (size_t i = 0; i < 6; ++i)
        {
            const auto& filename = filenames[i];

            IO::Log::Message("Load texture: \"" + filename + "\"");
            IO::Log::ScopedIndent indent;

            /* Read image from file */
            auto image = ImageFileHandler::ReadImage(filename);
            images.push_back(image);

            if (image)
            {
                FilterMaxImageSize(image->GetSize(), maxImageSize, mustResize);
                FilterCommonColorFormat(image->GetFormat(), commonColorFormat);
            }
        }

        if (mustResize)
            ResizeAllImages(images, maxImageSize);

        /* Setup texture buffer */
        const auto texSize = maxImageSize.Sz2().Cast<int>();

        renderSystem->WriteTexture(
            texture.get(),
            ChooseTextureFormat(commonColorFormat, RendererDataTypes::UByte),
            texSize
        );

        /* Initialize texture with image data */
        size_t faceIndex = 0;

        for (const auto& image : images)
        {
            if (image)
            {
                /* Adjust format alignment (for specific renderer requirements) */
                //if (renderSystem->RequireColorFormatAlignment())
                image->AdjustFormatAlignment();

                /* Write image data to texture */
                renderSystem->WriteSubTexture(
                    texture.get(), { 0, 0 }, texSize, cubeFaces[faceIndex], 0,
                    image->GetFormat(), RendererDataTypes::UByte, image->RawBuffer()
                );
            }
            ++faceIndex;
        }
    }

    renderContext->GenerateMIPMaps(texture.get());

    return AddTexture(texture);
}

Texture2DPtr TextureManager::LoadTexture2D(const std::string& filename, bool hasHDR)
{
    /* Check if texture already exists */
    auto texture = loaded2DTextures_.Find(filename);
    if (texture)
        return texture;

    /* Otherwise load texture from file */
    texture = CreateTexture2DFromFile(filename);
    if (texture)
    {
        loaded2DTextures_.hashMap[filename] = texture;
        return texture;
    }

    return nullptr;
}

void TextureManager::ReleaseTexture(const Texture* texture)
{
    /* Remove texture from hash map */
    loaded2DTextures_.Remove(texture);

    /* Remove texture from list */
    for (auto it = textures_.begin(); it != textures_.end(); ++it)
    {
        if (it->get() == texture)
        {
            textures_.erase(it);
            break;
        }
    }
}

void TextureManager::ReleaseAllTextures()
{
    loaded2DTextures_.hashMap.clear();
    textures_.clear();
}

void TextureManager::AddEventHandler(const EventHandlerPtr& eventHandler)
{
    if (eventHandler)
        eventHandlers_.push_back(eventHandler);
}

void TextureManager::RemoveEventHandler(const EventHandlerPtr& eventHandler)
{
    RemoveAllFromList(eventHandlers_, eventHandler);
}


} // /namespace Video

} // /namespace Fork



// ========================