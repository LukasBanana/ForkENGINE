/*
 * OpenGL constant buffer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLConstantBuffer.h"


namespace Fork
{

namespace Video
{


GLConstantBuffer::GLConstantBuffer(const Description& desc) :
    ConstantBuffer{ desc }
{
}
GLConstantBuffer::~GLConstantBuffer()
{
}


} // /namespace Video

} // /namespace Fork



// ========================