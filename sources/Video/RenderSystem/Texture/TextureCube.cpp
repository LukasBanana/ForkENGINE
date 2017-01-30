/*
 * Texture cube file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/Texture/TextureCube.h"


namespace Fork
{

namespace Video
{


TextureCube::~TextureCube()
{
}

Texture::Types TextureCube::Type() const
{
    return Types::TexCube;
}

TextureCube::SizeType TextureCube::MIPLevelSize(unsigned int mipLevel) const
{
    return
    {
        Video::MIPLevelSize(GetSize().width,  mipLevel),
        Video::MIPLevelSize(GetSize().height, mipLevel)
    };
}

unsigned int TextureCube::MapArrayLayer(unsigned int arrayIndex, const Faces face)
{
    return arrayIndex*6 + static_cast<unsigned int>(face);
}


} // /namespace Video

} // /namespace Fork



// ========================