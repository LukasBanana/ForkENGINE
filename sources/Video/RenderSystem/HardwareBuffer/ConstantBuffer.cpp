/*
 * Constant buffer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/HardwareBuffer/ConstantBuffer.h"


namespace Fork
{

namespace Video
{


ConstantBuffer::ConstantBuffer(const Description& desc) :
    desc_{ desc }
{
}
ConstantBuffer::~ConstantBuffer()
{
}


} // /namespace Video

} // /namespace Fork



// ========================