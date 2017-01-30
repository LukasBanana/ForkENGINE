/*
 * Direct3D 11 texture 3D file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11Texture3D.h"
#include "../D3D11Core.h"


namespace Fork
{

namespace Video
{


D3D11Texture3D::D3D11Texture3D()
{
}
D3D11Texture3D::~D3D11Texture3D()
{
    DXSafeRelease(resource_);
}

ID3D11Resource* D3D11Texture3D::AbstractResource()
{
    return GetResource();
}

bool D3D11Texture3D::CreateResource(
    ID3D11Device* device, const D3D11_TEXTURE3D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData)
{
    DXSafeRelease(resource_);
    return DXAssert(
        device->CreateTexture3D(&desc, initialData, &resource_),
        "Creating D3D11 3D-texture failed",
        false
    );
}


} // /namespace Video

} // /namespace Fork



// ========================