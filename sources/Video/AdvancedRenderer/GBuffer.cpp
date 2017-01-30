/*
 * G-buffer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/AdvancedRenderer/GBuffer.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "../RenderSystem/RenderSysCtx.h"


namespace Fork
{

namespace Video
{


GBuffer::GBuffer(
    const TextureType::SizeType& initSize, const TextureFormats colorMapFormat,
    const TextureFormats normalMapFormat, const TextureFormats depthMapFormat)
{
    colorMap    = RenderSys()->CreateTexture2D(colorMapFormat, initSize);
    normalMap   = RenderSys()->CreateTexture2D(normalMapFormat, initSize);
    depthMap    = RenderSys()->CreateTexture2D(depthMapFormat, initSize);

    renderTarget = RenderSys()->CreateRenderTarget({ colorMap, normalMap, depthMap });
}
GBuffer::~GBuffer()
{
}

void GBuffer::Resize(const TextureType::SizeType& size)
{
    renderTarget->Clear();

    RenderSys()->WriteTexture(colorMap.get(), colorMap->GetFormat(), size);
    RenderSys()->WriteTexture(normalMap.get(), normalMap->GetFormat(), size);
    RenderSys()->WriteTexture(depthMap.get(), depthMap->GetFormat(), size);

    renderTarget->Attach({ colorMap, normalMap, depthMap });
}


} // /namespace Video

} // /namespace Fork



// ========================