/*
 * Storage buffer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/HardwareBuffer/StorageBuffer.h"


namespace Fork
{

namespace Video
{


StorageBuffer::StorageBuffer(const Description& desc) :
    desc_{ desc }
{
}
StorageBuffer::~StorageBuffer()
{
}


} // /namespace Video

} // /namespace Fork



// ========================