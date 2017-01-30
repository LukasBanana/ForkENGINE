/*
 * Direct3D 11 texture 1D file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11Texture1D.h"
#include "../D3D11Core.h"


namespace Fork
{

namespace Video
{


D3D11Texture1D::D3D11Texture1D()
{
}
D3D11Texture1D::~D3D11Texture1D()
{
    DXSafeRelease(resource_);
}

ID3D11Resource* D3D11Texture1D::AbstractResource()
{
    return GetResource();
}

bool D3D11Texture1D::CreateResource(
    ID3D11Device* device, const D3D11_TEXTURE1D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData)
{
    DXSafeRelease(resource_);
    return DXAssert(
        device->CreateTexture1D(&desc, initialData, &resource_),
        "Creating D3D11 1D-texture failed",
        false
    );
}


/*
 * ======= Private: =======
 */

void D3D11Texture1D::SetupArray(unsigned int arraySize)
{
    if (arraySize > 0)
    {
        this->arraySize = arraySize;
        //...
    }
    else
    {
        this->arraySize = 0;
        //...
    }
}


} // /namespace Video

} // /namespace Fork



// ========================