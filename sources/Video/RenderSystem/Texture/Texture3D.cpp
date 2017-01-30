/*
 * Texture 3D file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/Texture/Texture3D.h"


namespace Fork
{

namespace Video
{


Texture3D::~Texture3D()
{
}

Texture::Types Texture3D::Type() const
{
    return Types::Tex3D;
}

Texture3D::SizeType Texture3D::MIPLevelSize(unsigned int mipLevel) const
{
    return
    {
        Video::MIPLevelSize(GetSize().width,  mipLevel),
        Video::MIPLevelSize(GetSize().height, mipLevel),
        Video::MIPLevelSize(GetSize().depth,  mipLevel)
    };
}


} // /namespace Video

} // /namespace Fork



// ========================