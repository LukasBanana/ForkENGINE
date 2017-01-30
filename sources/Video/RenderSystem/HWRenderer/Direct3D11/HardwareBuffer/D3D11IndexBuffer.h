/*
 * Direct3D 11 index buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_INDEX_BUFFER_H__
#define __FORK_D3D11_INDEX_BUFFER_H__


#include "Video/RenderSystem/HardwareBuffer/IndexBuffer.h"
#include "D3D11HardwareBuffer.h"


namespace Fork
{

namespace Video
{


class D3D11IndexBuffer : public IndexBuffer, public D3D11HardwareBuffer
{
    
    public:
        
        D3D11IndexBuffer() = default;

        D3D11IndexBuffer(const D3D11IndexBuffer&) = delete;
        D3D11IndexBuffer& operator = (const D3D11IndexBuffer&) = delete;

        HRESULT WriteIndices(
            ID3D11Device* device, const void* data, UINT size,
            D3D11_USAGE usage = D3D11_USAGE_DEFAULT
        );

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================