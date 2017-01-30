/*
 * Direct3D 11 texture cube file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11TextureCube.h"
#include "../D3D11Core.h"
#include "../D3D11ParamMapper.h"
#include "Core/STLHelper.h"


namespace Fork
{

namespace Video
{


D3D11TextureCube::D3D11TextureCube()
{
}
D3D11TextureCube::~D3D11TextureCube()
{
    DXSafeRelease(resource_);
}

ID3D11Resource* D3D11TextureCube::AbstractResource()
{
    return GetResource();
}

bool D3D11TextureCube::CreateResource(
    ID3D11Device* device, const D3D11_TEXTURE2D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData)
{
    DXSafeRelease(resource_);

    /* Create texture resource */
    auto result = device->CreateTexture2D(&desc, initialData, &resource_);
    if (!DXAssert(result, "Creating D3D11 cube-texture failed", false))
        return false;

    /* Create shader resource view */
    D3D11_SHADER_RESOURCE_VIEW_DESC resourceView;
    InitRawMemory(resourceView);

    resourceView.Format = D3D11ParamMapper::Map(GetFormat());

    if (HasArray())
    {
        resourceView.ViewDimension                      = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
        resourceView.TextureCubeArray.MostDetailedMip   = 0;
        resourceView.TextureCubeArray.MipLevels         = -1;
        resourceView.TextureCubeArray.First2DArrayFace  = 0;
        resourceView.TextureCubeArray.NumCubes          = GetArraySize();
    }
    else
    {
        resourceView.ViewDimension                  = D3D11_SRV_DIMENSION_TEXTURECUBE;
        resourceView.TextureCube.MostDetailedMip    = 0;
        resourceView.TextureCube.MipLevels          = -1;
    }

    return CreateResourceView(device, &resourceView);
}


/*
 * ======= Protected: =======
 */

void D3D11TextureCube::SetupArray(unsigned int arraySize)
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

void D3D11TextureCube::SetupSize(const TextureCube::SizeType& size)
{
    this->size = size;
}


} // /namespace Video

} // /namespace Fork



// ========================