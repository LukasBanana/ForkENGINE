/*
 * Direct3D 11 storage buffer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11StorageBuffer.h"


namespace Fork
{

namespace Video
{


D3D11StorageBuffer::D3D11StorageBuffer(ID3D11Device* device, const Description& desc) :
    StorageBuffer{ desc }
{
}


} // /namespace Video

} // /namespace Fork



// ========================