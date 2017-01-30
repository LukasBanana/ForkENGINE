/*
 * Image array container file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/AdvancedRenderer/ImageArrayContainer.h"


namespace Fork
{

namespace Video
{


ImageArrayContainer::~ImageArrayContainer()
{
}

void ImageArrayContainer::SetupAndDrawImageArray(
    PrimitiveRenderer& renderer, const Math::Point2f& offsetPosition,
    const Math::Size2f& baseSize, const Video::ColorRGBAf& baseColor)
{
    if (!GetImageEntries().empty())
    {
        renderer.SetupImageArray(GetImageEntries());
        renderer.DrawImageArray(GetTexture(), offsetPosition, baseSize, baseColor);
    }
}

const PrimitiveRenderer::ImageArrayEntry* ImageArrayContainer::ImageEntry(size_t index) const
{
    return index < imageEntries_.size() ? &(imageEntries_[index]) : nullptr;
}


} // /namespace Video

} // /namespace Fork



// ========================