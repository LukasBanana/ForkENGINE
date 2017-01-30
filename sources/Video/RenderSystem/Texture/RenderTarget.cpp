/*
 * Render target file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/Texture/RenderTarget.h"
#include "Video/RenderSystem/Texture/TextureMentor.h"
#include "Core/StaticConfig.h"
#include "IO/Core/Log.h"


namespace Fork
{

namespace Video
{


RenderTarget::~RenderTarget()
{
}

size_t RenderTarget::Attach(const std::vector<TexturePtr>& textures)
{
    /* Check if textures have all the same size */
    if (textures.empty())
        return 0;

    #ifdef FORK_DEBUG

    /* Query first texture size */
    auto desc = TextureMentor::QueryTextureDesc(textures.front().get());
    const Math::Size2i firstSize { desc.size.width, desc.size.height };

    auto it = textures.begin();
    for (++it; it != textures.end(); ++it)
    {
        /* Query texture size to compare with the first size */
        desc = TextureMentor::QueryTextureDesc(it->get());
        const Math::Size2i compSize { desc.size.width, desc.size.height };

        if (firstSize != compSize)
        {
            /* Print debug message and cancel comparision */
            IO::Log::Debug("RenderTarget::Attach", "Varying texture sizes in render target attachments");
            break;
        }
    }

    #endif

    /* Attach all textures to the render target */
    size_t numAttachedTextures = 0;

    unsigned int attachment = 0;
    unsigned int cubeFaceIndex = 0;

    for (auto tex : textures)
    {
        bool result = false;

        switch (tex->Type())
        {
            case Texture::Types::Tex1D:
            {
                result = Attach(std::static_pointer_cast<Texture1D>(tex), attachment);
            }
            break;

            case Texture::Types::Tex2D:
            {
                result = Attach(std::static_pointer_cast<Texture2D>(tex), attachment);
            }
            break;

            case Texture::Types::Tex3D:
            {
                result = Attach(std::static_pointer_cast<Texture3D>(tex), attachment);
            }
            break;

            case Texture::Types::TexCube:
            {
                result = Attach(
                    std::static_pointer_cast<TextureCube>(tex),
                    static_cast<TextureCube::Faces>(cubeFaceIndex), attachment
                );

                if (++cubeFaceIndex >= 6)
                    cubeFaceIndex = 0;
            }
            break;
        }

        if (result)
            ++numAttachedTextures;

        ++attachment;
    }

    return numAttachedTextures;
}


/*
 * ======= Protected: =======
 */

bool RenderTarget::ValidateMIPLevelInput(unsigned int mipLevel, bool hasMIPMaps) const
{
    if (mipLevel > 0 && !hasMIPMaps)
    {
        #ifdef FORK_DEBUG
        IO::Log::Debug(
            "RenderTarget::Attach",
            "Cannot attach MIP level to render-target when no MIP maps have been generated",
            10
        );
        #endif
        return false;
    }
    return true;
}


} // /namespace Video

} // /namespace Fork



// ========================