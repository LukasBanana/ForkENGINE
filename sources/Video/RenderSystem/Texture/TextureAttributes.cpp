/*
 * Texture attributes file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/Texture/TextureAttributes.h"

#include <algorithm>


namespace Fork
{

namespace Video
{


FORK_EXPORT int NumColorComponents(const TextureFormats format)
{
    switch (BaseTextureFormat(format))
    {
        case TextureFormats::DepthComponent:
            return 1;
        case TextureFormats::DepthStencil:
            return 2;
        case TextureFormats::R:
            return 1;
        case TextureFormats::RG:
            return 2;
        case TextureFormats::RGB:
            return 3;
        case TextureFormats::RGBA:
            return 4;
    }
    return 0;
}

FORK_EXPORT int NumColorComponentBits(const TextureFormats format)
{
    switch (format)
    {
        /* --- Base internal formats --- */
        case TextureFormats::DepthComponent:
        case TextureFormats::DepthStencil:
            return 32;

        case TextureFormats::R:
        case TextureFormats::RG:
        case TextureFormats::RGB:
        case TextureFormats::RGBA:
            return 8;

        /* --- Sized internal formats --- */
        case TextureFormats::R8UInt:
        case TextureFormats::R8SInt:
        case TextureFormats::RG8UInt:
        case TextureFormats::RG8SInt:
        case TextureFormats::RGB8UInt:
        case TextureFormats::RGB8SInt:
        case TextureFormats::RGBA8UInt:
        case TextureFormats::RGBA8SInt:
            return 8;

        case TextureFormats::R16UInt:
        case TextureFormats::R16SInt:
        case TextureFormats::R16Float:
        case TextureFormats::RG16UInt:
        case TextureFormats::RG16SInt:
        case TextureFormats::RG16Float:
        case TextureFormats::RGB16UInt:
        case TextureFormats::RGB16SInt:
        case TextureFormats::RGB16Float:
        case TextureFormats::RGBA16UInt:
        case TextureFormats::RGBA16SInt:
        case TextureFormats::RGBA16Float:
            return 16;

        case TextureFormats::R32UInt:
        case TextureFormats::R32SInt:
        case TextureFormats::R32Float:
        case TextureFormats::RG32UInt:
        case TextureFormats::RG32SInt:
        case TextureFormats::RG32Float:
        case TextureFormats::RGB32UInt:
        case TextureFormats::RGB32SInt:
        case TextureFormats::RGB32Float:
        case TextureFormats::RGBA32UInt:
        case TextureFormats::RGBA32SInt:
        case TextureFormats::RGBA32Float:
            return 32;
    }
    return 0;
}

FORK_EXPORT TextureFormats BaseTextureFormat(const TextureFormats format)
{
    switch (format)
    {
        /* --- Sized internal formats --- */
        case TextureFormats::R8UInt:
        case TextureFormats::R8SInt:
        case TextureFormats::R16UInt:
        case TextureFormats::R16SInt:
        case TextureFormats::R16Float:
        case TextureFormats::R32UInt:
        case TextureFormats::R32SInt:
        case TextureFormats::R32Float:
            return TextureFormats::R;

        case TextureFormats::RG8UInt:
        case TextureFormats::RG8SInt:
        case TextureFormats::RG16UInt:
        case TextureFormats::RG16SInt:
        case TextureFormats::RG16Float:
        case TextureFormats::RG32UInt:
        case TextureFormats::RG32SInt:
        case TextureFormats::RG32Float:
            return TextureFormats::RG;

        case TextureFormats::RGB8UInt:
        case TextureFormats::RGB8SInt:
        case TextureFormats::RGB16UInt:
        case TextureFormats::RGB16SInt:
        case TextureFormats::RGB16Float:
        case TextureFormats::RGB32UInt:
        case TextureFormats::RGB32SInt:
        case TextureFormats::RGB32Float:
            return TextureFormats::RGB;

        case TextureFormats::RGBA8UInt:
        case TextureFormats::RGBA8SInt:
        case TextureFormats::RGBA16UInt:
        case TextureFormats::RGBA16SInt:
        case TextureFormats::RGBA16Float:
        case TextureFormats::RGBA32UInt:
        case TextureFormats::RGBA32SInt:
        case TextureFormats::RGBA32Float:
            return TextureFormats::RGBA;

        default:
            break;
    }
    return format;
}

FORK_EXPORT TextureFormats ChooseTextureFormat(const ImageColorFormats imageFormat, const RendererDataTypes dataType)
{
    switch (imageFormat)
    {
        case ImageColorFormats::Gray:
            switch (dataType)
            {
                case RendererDataTypes::Byte:
                    return TextureFormats::R8SInt;
                case RendererDataTypes::UByte:
                    return TextureFormats::R8UInt;
                case RendererDataTypes::Short:
                    return TextureFormats::R16SInt;
                case RendererDataTypes::UShort:
                    return TextureFormats::R16UInt;
                case RendererDataTypes::Int:
                    return TextureFormats::R32SInt;
                case RendererDataTypes::UInt:
                    return TextureFormats::R32UInt;
                case RendererDataTypes::Float:
                    return TextureFormats::R32Float;
                default:
                    break;
            }
            break;

        case ImageColorFormats::GrayAlpha:
            switch (dataType)
            {
                case RendererDataTypes::Byte:
                    return TextureFormats::RG8SInt;
                case RendererDataTypes::UByte:
                    return TextureFormats::RG8UInt;
                case RendererDataTypes::Short:
                    return TextureFormats::RG16SInt;
                case RendererDataTypes::UShort:
                    return TextureFormats::RG16UInt;
                case RendererDataTypes::Int:
                    return TextureFormats::RG32SInt;
                case RendererDataTypes::UInt:
                    return TextureFormats::RG32UInt;
                case RendererDataTypes::Float:
                    return TextureFormats::RG32Float;
                default:
                    break;
            }
            break;

        case ImageColorFormats::RGB:
        case ImageColorFormats::BGR:
            switch (dataType)
            {
                case RendererDataTypes::Byte:
                    return TextureFormats::RGB8SInt;
                case RendererDataTypes::UByte:
                    return TextureFormats::RGB8UInt;
                case RendererDataTypes::Short:
                    return TextureFormats::RGB16SInt;
                case RendererDataTypes::UShort:
                    return TextureFormats::RGB16UInt;
                case RendererDataTypes::Int:
                    return TextureFormats::RGB32SInt;
                case RendererDataTypes::UInt:
                    return TextureFormats::RGB32UInt;
                case RendererDataTypes::Float:
                    return TextureFormats::RGB32Float;
                default:
                    break;
            }
            break;

        case ImageColorFormats::RGBA:
        case ImageColorFormats::BGRA:
            switch (dataType)
            {
                case RendererDataTypes::Byte:
                    return TextureFormats::RGBA8SInt;
                case RendererDataTypes::UByte:
                    return TextureFormats::RGBA8UInt;
                case RendererDataTypes::Short:
                    return TextureFormats::RGBA16SInt;
                case RendererDataTypes::UShort:
                    return TextureFormats::RGBA16UInt;
                case RendererDataTypes::Int:
                    return TextureFormats::RGBA32SInt;
                case RendererDataTypes::UInt:
                    return TextureFormats::RGBA32UInt;
                case RendererDataTypes::Float:
                    return TextureFormats::RGBA32Float;
                default:
                    break;
            }
            break;

        case ImageColorFormats::DepthComponent:
            return TextureFormats::DepthComponent;
        case ImageColorFormats::DepthStencil:
            return TextureFormats::DepthStencil;
    }
    return TextureFormats::RGBA;
}

FORK_EXPORT int MIPLevelSize(int size, unsigned int mipLevel)
{
    return std::max(1, size >> mipLevel);
}


} // /namespace Video

} // /namespace Fork



// ========================