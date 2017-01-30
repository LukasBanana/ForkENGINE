/*
 * Texture mentor file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/Texture/TextureMentor.h"
#include "Video/RenderSystem/Texture/Texture1D.h"
#include "Video/RenderSystem/Texture/Texture2D.h"
#include "Video/RenderSystem/Texture/Texture3D.h"
#include "Video/RenderSystem/Texture/TextureCube.h"
#include "Core/Exception/NullPointerException.h"


namespace Fork
{

namespace Video
{

namespace TextureMentor
{


static void QueryTexture1DDesc(const Texture1D* texture, Texture::Description& desc)
{
    desc.arraySize  = texture->GetArraySize();
    desc.size       = { texture->GetSize(), 1, 1 };
}

static void QueryTexture2DDesc(const Texture2D* texture, Texture::Description& desc)
{
    desc.arraySize  = texture->GetArraySize();
    desc.size       = { texture->GetSize().width, texture->GetSize().height, 1 };
}

static void QueryTexture3DDesc(const Texture3D* texture, Texture::Description& desc)
{
    desc.arraySize  = 0;
    desc.size       = texture->GetSize();
}

static void QueryTextureCubeDesc(const TextureCube* texture, Texture::Description& desc)
{
    desc.arraySize  = texture->GetArraySize();
    desc.size       = { texture->GetSize().width, texture->GetSize().height, 6 };
}

FORK_EXPORT Texture::Description QueryTextureDesc(const Texture* texture)
{
    ASSERT_POINTER(texture);
    
    Texture::Description desc;

    /* Query basic texture description */
    desc.format     = texture->GetFormat();
    desc.hasMIPMaps = texture->HasMIPMaps();

    /* Query extended texture description */
    switch (texture->Type())
    {
        case Texture::Types::Tex1D:
            QueryTexture1DDesc(static_cast<const Texture1D*>(texture), desc);
            break;
        case Texture::Types::Tex2D:
            QueryTexture2DDesc(static_cast<const Texture2D*>(texture), desc);
            break;
        case Texture::Types::Tex3D:
            QueryTexture3DDesc(static_cast<const Texture3D*>(texture), desc);
            break;
        case Texture::Types::TexCube:
            QueryTextureCubeDesc(static_cast<const TextureCube*>(texture), desc);
            break;
    }

    return desc;
}


} // /namespace TextureMentor

} // /namespace Video

} // /namespace Fork



// ========================