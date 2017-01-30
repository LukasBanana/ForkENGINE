/*
 * Direct3D 11 harware buffer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11HardwareBuffer.h"
#include "../D3D11Core.h"
#include "Core/STLHelper.h"

#include <algorithm>


namespace Fork
{

namespace Video
{


D3D11HardwareBuffer::D3D11HardwareBuffer()
{
}
D3D11HardwareBuffer::~D3D11HardwareBuffer()
{
    DXSafeRelease(buffer_);
}

HRESULT D3D11HardwareBuffer::Write(
    ID3D11Device* device, const void* data, UINT size, UINT bindFlags,
    UINT stride, UINT cpuAccessFlags, UINT miscFlags, D3D11_USAGE usage)
{
    /* Release previous buffer */
    DXSafeRelease(buffer_);

    /* Setup buffer description */
    D3D11_BUFFER_DESC bufferDesc;
    InitRawMemory(bufferDesc);

    bufferDesc.ByteWidth            = std::max(1u, size);
    bufferDesc.Usage                = usage;
    bufferDesc.BindFlags            = bindFlags;
    bufferDesc.CPUAccessFlags       = cpuAccessFlags;
    bufferDesc.MiscFlags            = miscFlags;
    bufferDesc.StructureByteStride  = stride;

    /* Setup resource data */
    D3D11_SUBRESOURCE_DATA resourceData;

    if (data)
    {
        InitRawMemory(resourceData);
        resourceData.pSysMem = data;
    }

    /* Create buffer object */
    return device->CreateBuffer(
        &bufferDesc,
        data != nullptr ? &resourceData : nullptr,
        &buffer_
    );
}

void D3D11HardwareBuffer::WriteSub(
    ID3D11DeviceContext* context, const void* data,
    UINT srcRowPitch, UINT srcDepthPitch, const D3D11_BOX& destBox)
{
    if (buffer_)
        context->UpdateSubresource(buffer_, 0, &destBox, data, srcRowPitch, srcDepthPitch);
}

void D3D11HardwareBuffer::WriteSub(
    ID3D11DeviceContext* context, const void* data, UINT offset, UINT size)
{
    WriteSub(context, data, 0, 0, { offset, 0, 0, offset + size, 1, 1 });
}

void D3D11HardwareBuffer::WriteSub(ID3D11DeviceContext* context, const void* data)
{
    if (buffer_)
        context->UpdateSubresource(buffer_, 0, nullptr, data, 0, 0);
}


} // /namespace Video

} // /namespace Fork



// ========================