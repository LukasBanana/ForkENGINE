/*
 * OpenGL storage buffer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLStorageBuffer.h"


namespace Fork
{

namespace Video
{


GLStorageBuffer::GLStorageBuffer(const Description& desc) :
    StorageBuffer{ desc }
{
}
GLStorageBuffer::~GLStorageBuffer()
{
}


} // /namespace Video

} // /namespace Fork



// ========================