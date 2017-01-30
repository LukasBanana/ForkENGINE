/*
 * Texture 2D file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/Texture/Texture2D.h"
#include "Video/RenderSystem/Texture/TextureAttributes.h"


namespace Fork
{

namespace Video
{


Texture2D::~Texture2D()
{
}

Texture::Types Texture2D::Type() const
{
    return Types::Tex2D;
}

Texture2D::SizeType Texture2D::MIPLevelSize(unsigned int mipLevel) const
{
    return
    {
        Video::MIPLevelSize(GetSize().width,  mipLevel),
        Video::MIPLevelSize(GetSize().height, mipLevel)
    };
}


} // /namespace Video

} // /namespace Fork



// ========================