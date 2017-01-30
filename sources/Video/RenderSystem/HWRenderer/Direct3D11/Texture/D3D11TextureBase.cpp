/*
 * Direct3D 11 texture base file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11TextureBase.h"
#include "../D3D11Core.h"


namespace Fork
{

namespace Video
{


D3D11TextureBase::~D3D11TextureBase()
{
    DXSafeRelease(resourceView_);
}

bool D3D11TextureBase::CreateResourceView(ID3D11Device* device, const D3D11_SHADER_RESOURCE_VIEW_DESC* desc)
{
    DXSafeRelease(resourceView_);
    return DXAssert(
        device->CreateShaderResourceView(AbstractResource(), desc, &resourceView_),
        "Creating D3D11 shader resource view for texture failed",
        false
    );
}


} // /namespace Video

} // /namespace Fork



// ========================