/*
 * Direct3D 11 constant buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_CONSTANT_BUFFER_H__
#define __FORK_D3D11_CONSTANT_BUFFER_H__


#include "Video/RenderSystem/HardwareBuffer/ConstantBuffer.h"
#include "D3D11HardwareBuffer.h"


namespace Fork
{

namespace Video
{


//! Direct3D 11 shader constant buffer implementation.
class D3D11ConstantBuffer : public ConstantBuffer, public D3D11HardwareBuffer
{
    
    public:
        
        D3D11ConstantBuffer(ID3D11Device* device, const Description& desc);

        D3D11ConstantBuffer(const D3D11ConstantBuffer&) = delete;
        D3D11ConstantBuffer& operator = (const D3D11ConstantBuffer&) = delete;

        HRESULT WriteConstants(
            ID3D11Device* device, const void* data, UINT size,
            D3D11_USAGE usage = D3D11_USAGE_DEFAULT
        );

        void WriteSub(ID3D11DeviceContext* context, const void* data, UINT offset, UINT size);

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================