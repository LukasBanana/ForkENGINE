/*
 * Direct3D 11 storage buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_STORAGE_BUFFER_H__
#define __FORK_D3D11_STORAGE_BUFFER_H__


#include "Video/RenderSystem/HardwareBuffer/StorageBuffer.h"
#include "D3D11HardwareBuffer.h"


namespace Fork
{

namespace Video
{


/**
Direct3D 11 shader constant buffer implementation.
\todo Not yet implemented!
*/
class D3D11StorageBuffer : public StorageBuffer, public D3D11HardwareBuffer
{
    
    public:
        
        D3D11StorageBuffer(ID3D11Device* device, const Description& desc);

        D3D11StorageBuffer(const D3D11StorageBuffer&) = delete;
        D3D11StorageBuffer& operator = (const D3D11StorageBuffer&) = delete;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================