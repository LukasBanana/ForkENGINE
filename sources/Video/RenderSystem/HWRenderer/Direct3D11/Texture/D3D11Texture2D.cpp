/*
 * Direct3D 11 texture 2D file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11Texture2D.h"
#include "../D3D11Core.h"
#include "Core/STLHelper.h"


namespace Fork
{

namespace Video
{


D3D11Texture2D::D3D11Texture2D()
{
}
D3D11Texture2D::~D3D11Texture2D()
{
    DXSafeRelease(resource_);
}

ID3D11Resource* D3D11Texture2D::AbstractResource()
{
    return GetResource();
}

bool D3D11Texture2D::CreateResource(
    ID3D11Device* device, const D3D11_TEXTURE2D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData)
{
    DXSafeRelease(resource_);

    /* Create texture resource */
    auto result = device->CreateTexture2D(&desc, initialData, &resource_);
    if (!DXAssert(result, "Creating D3D11 2D-texture failed", false))
        return false;

    /* Create shader resource view */
    return CreateResourceView(device);
}


/*
 * ======= Private: =======
 */

void D3D11Texture2D::SetupArray(unsigned int arraySize)
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

void D3D11Texture2D::SetupSize(const Texture2D::SizeType& size)
{
    this->size = size;
}


} // /namespace Video

} // /namespace Fork



// ========================