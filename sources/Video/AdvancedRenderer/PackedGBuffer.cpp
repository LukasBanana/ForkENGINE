/*
 * Packed g-buffer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/AdvancedRenderer/PackedGBuffer.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "../RenderSystem/RenderSysCtx.h"


namespace Fork
{

namespace Video
{


PackedGBuffer::PackedGBuffer(
    const TextureType::SizeType& initSize,
    const TextureFormats colorAndSpecularMapFormat, const TextureFormats normalAndDepthMapFormat)
{
    colorAndSpecularMap = RenderSys()->CreateTexture2D(colorAndSpecularMapFormat, initSize);
    normalAndDepthMap   = RenderSys()->CreateTexture2D(normalAndDepthMapFormat, initSize);

    renderTarget = RenderSys()->CreateRenderTarget({ colorAndSpecularMap, normalAndDepthMap });
}
PackedGBuffer::~PackedGBuffer()
{
}

void PackedGBuffer::Resize(const TextureType::SizeType& size)
{
    renderTarget->Clear();

    RenderSys()->WriteTexture(colorAndSpecularMap.get(), colorAndSpecularMap->GetFormat(), size);
    RenderSys()->WriteTexture(normalAndDepthMap.get(), normalAndDepthMap->GetFormat(), size);

    renderTarget->Attach(colorAndSpecularMap);
    renderTarget->Attach(normalAndDepthMap);
}


} // /namespace Video

} // /namespace Fork



// ========================