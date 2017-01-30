/*
 * Packed cube g-buffer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/AdvancedRenderer/PackedCubeGBuffer.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "../RenderSystem/RenderSysCtx.h"


namespace Fork
{

namespace Video
{


PackedCubeGBuffer::PackedCubeGBuffer(
    const TextureType::SizeType& initSize,
    const TextureFormats colorAndSpecularMapFormat, const TextureFormats normalAndDepthMapFormat)
{
    colorAndSpecularMap = RenderSys()->CreateTextureCube(colorAndSpecularMapFormat, initSize);
    normalAndDepthMap   = RenderSys()->CreateTextureCube(normalAndDepthMapFormat, initSize);

    cubeRenderTargets = RenderSys()->CreateCubeRenderTargets({ colorAndSpecularMap, normalAndDepthMap });

    for (size_t i = 0; i < 6; ++i)
        cubeRenderTargetRefs[i] = cubeRenderTargets[i].get();
}
PackedCubeGBuffer::~PackedCubeGBuffer()
{
}

void PackedCubeGBuffer::Resize(const TextureType::SizeType& size)
{
    for (size_t i = 0; i < 6; ++i)
        cubeRenderTargetRefs[i]->Clear();

    RenderSys()->WriteTexture(colorAndSpecularMap.get(), colorAndSpecularMap->GetFormat(), size);
    RenderSys()->WriteTexture(normalAndDepthMap.get(), normalAndDepthMap->GetFormat(), size);

    for (size_t i = 0; i < 6; ++i)
    {
        auto cubeFace = static_cast<Video::TextureCube::Faces>(i);
        cubeRenderTargetRefs[i]->Attach(colorAndSpecularMap, cubeFace, 0);
        cubeRenderTargetRefs[i]->Attach(normalAndDepthMap, cubeFace, 1);
    }
}


} // /namespace Video

} // /namespace Fork



// ========================