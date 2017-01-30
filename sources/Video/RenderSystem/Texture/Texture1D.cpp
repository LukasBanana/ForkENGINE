/*
 * Texture 1D file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/Texture/Texture1D.h"

#include <cmath>


namespace Fork
{

namespace Video
{


Texture1D::~Texture1D()
{
}

Texture::Types Texture1D::Type() const
{
    return Types::Tex1D;
}

Texture1D::SizeType Texture1D::MIPLevelSize(unsigned int mipLevel) const
{
    return Video::MIPLevelSize(GetSize(),  mipLevel);
}


} // /namespace Video

} // /namespace Fork



// ========================