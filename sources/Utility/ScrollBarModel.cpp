/*
 * Scroll bar model file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/ScrollBarModel.h"
#include "Math/Core/MathConstants.h"
#include "Math/Core/BaseMath.h"

#include <algorithm>


namespace Fork
{

namespace Utility
{


ScrollBarModel::ScrollBarModel(float size, float areaSize, float scrollPos) :
    size_       { size      },
    areaSize_   { areaSize  },
    scrollPos_  { scrollPos }
{
}
ScrollBarModel::~ScrollBarModel()
{
}

void ScrollBarModel::SetupSize(float size)
{
    size_ = std::max(0.0f, size);
}

void ScrollBarModel::SetupAreaSize(float areaSize)
{
    areaSize_ = std::max(0.0f, areaSize);
}

void ScrollBarModel::SetupMinBarSize(float minBarSize)
{
    minBarSize_ = std::max(0.0f, minBarSize);
}

void ScrollBarModel::SetupScrollPosition(float position)
{
    scrollPos_ = Math::Saturate(position);
}

void ScrollBarModel::Scroll(float direction)
{
    if (CanScroll())
    {
        const float nonVisibleSize = GetAreaSize() - GetSize();
        SetupScrollPosition(GetScrollPosition() + direction / nonVisibleSize);
    }
}

void ScrollBarModel::ScrollBegin()
{
    scrollPos_ = 0.0f;
}

void ScrollBarModel::ScrollEnd()
{
    scrollPos_ = 1.0f;
}

void ScrollBarModel::ScrollPageForwards()
{
    Scroll(GetSize());
}

void ScrollBarModel::ScrollPageBackwards()
{
    Scroll(-GetSize());
}

bool ScrollBarModel::IsScrollBegin() const
{
    return GetScrollPosition() <= Math::epsilon;
}

bool ScrollBarModel::IsScrollEnd() const
{
    return GetScrollPosition() >= 1.0f - Math::epsilon;
}

bool ScrollBarModel::CanScroll() const
{
    return GetAreaSize() > GetSize();
}

float ScrollBarModel::BarSize(float visualSize) const
{
    return BarSize() * visualSize / GetSize();
}

float ScrollBarModel::BarSize() const
{
    if (CanScroll())
    {
        auto barSizePercent = GetSize() / GetAreaSize();
        return std::max(GetMinBarSize(), GetSize() * barSizePercent);
    }
    return GetSize();
}

float ScrollBarModel::BarPosition(bool isReverse, float visualSize) const
{
    return BarPosition(isReverse) * visualSize / GetSize();
}

float ScrollBarModel::BarPosition(bool isReverse) const
{
    if (CanScroll())
        return (isReverse ? 1.0f - GetScrollPosition() : GetScrollPosition()) * ScrollArea();
    return 0.0f;
}

float ScrollBarModel::ScrollOffset() const
{
    if (CanScroll())
    {
        auto nonVisibleSize = GetAreaSize() - GetSize();
        return GetScrollPosition() * nonVisibleSize;
    }
    return 0.0f;
}

float ScrollBarModel::ScrollArea() const
{
    if (CanScroll())
    {
        auto barSizePercent = GetSize() / GetAreaSize();
        auto barSize = std::max(GetMinBarSize(), GetSize() * barSizePercent);
        return GetSize() - barSize;
    }
    return 0.0f;
}

void ScrollBarModel::SetupBarPosition(float relativePosition)
{
    if (CanScroll())
        SetupScrollPosition(relativePosition / ScrollArea());
}


} // /namespace Utility

} // /namespace Fork



// ========================