/*
 * Cube GBuffer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/AdvancedRenderer/CubeGBuffer.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "../RenderSystem/RenderSysCtx.h"


namespace Fork
{

namespace Video
{


CubeGBuffer::CubeGBuffer(
    const TextureType::SizeType& initSize, const TextureFormats colorMapFormat,
    const TextureFormats normalMapFormat, const TextureFormats depthMapFormat)
{
    colorMap    = RenderSys()->CreateTextureCube(colorMapFormat, initSize);
    normalMap   = RenderSys()->CreateTextureCube(normalMapFormat, initSize);
    depthMap    = RenderSys()->CreateTextureCube(depthMapFormat, initSize);

    cubeRenderTargets = RenderSys()->CreateCubeRenderTargets({ colorMap, normalMap, depthMap });

    for (size_t i = 0; i < 6; ++i)
        cubeRenderTargetRefs[i] = cubeRenderTargets[i].get();
}
CubeGBuffer::~CubeGBuffer()
{
}

void CubeGBuffer::Resize(const TextureType::SizeType& size)
{
    for (size_t i = 0; i < 6; ++i)
        cubeRenderTargetRefs[i]->Clear();

    RenderSys()->WriteTexture(colorMap.get(), colorMap->GetFormat(), size);
    RenderSys()->WriteTexture(normalMap.get(), normalMap->GetFormat(), size);
    RenderSys()->WriteTexture(depthMap.get(), depthMap->GetFormat(), size);

    for (size_t i = 0; i < 6; ++i)
    {
        auto cubeFace = static_cast<Video::TextureCube::Faces>(i);
        cubeRenderTargetRefs[i]->Attach(colorMap, cubeFace, 0);
        cubeRenderTargetRefs[i]->Attach(normalMap, cubeFace, 1);
        cubeRenderTargetRefs[i]->Attach(depthMap, cubeFace, 2);
    }
}


} // /namespace Video

} // /namespace Fork



// ========================