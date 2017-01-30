/*
 * Selection area model file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/SelectionAreaModel.h"


namespace Fork
{

namespace Utility
{


SelectionAreaModel::~SelectionAreaModel()
{
}

void SelectionAreaModel::StartDrag()
{
    isActive_ = true;
}

void SelectionAreaModel::StopDrag()
{
    isActive_ = false;
}

void SelectionAreaModel::Locate(const Math::Point2f& position)
{
    if (!isActive_)
        areaRect_.LocateLTPoint(position);
    areaRect_.LocateRBPoint(position);
}

Math::Rectf SelectionAreaModel::AreaRect() const
{
    auto rect = areaRect_;
    rect.Repair();
    return rect;
}

Math::Rectf SelectionAreaModel::operator () () const
{
    return AreaRect();
}

SelectionAreaModel::operator bool() const
{
    return IsActive();
}


} // /namespace Utility

} // /namespace Fork



// ========================