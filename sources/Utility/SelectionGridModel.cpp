/*
 * Selection grid model file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/SelectionGridModel.h"
#include "Math/Core/BaseMath.h"

#include <algorithm>


namespace Fork
{

namespace Utility
{


SelectionGridModel::~SelectionGridModel()
{
}

void SelectionGridModel::ResizeGrid(unsigned int numEntries, unsigned int numEntriesPerRow)
{
    /* Get previous selection index */
    auto prevIndex = SelectionIndex();

    /* Clamp number of entries per row */
    numEntriesPerRow = std::max(1u, numEntriesPerRow);

    /* Store new number of entries and clamp previous index */
    numEntries_ = numEntries;

    if (IsEmpty())
    {
        /* If grid is empty -> clear states */
        size_ = { 0, 0 };
        selectionPosition_ = { 0, 0 };
        return;
    }

    prevIndex = std::min(prevIndex, numEntries_ - 1);

    /* Update grid size */
    size_.width = numEntriesPerRow;
    size_.height = numEntries / numEntriesPerRow;

    if (numEntries % numEntriesPerRow > 0)
        ++size_.height;

    /* Relocate selection position by previous index */
    selectionPosition_ = PositionByIndex(prevIndex);
}

void SelectionGridModel::MoveSelection(const Math::Vector2i& direction)
{
    if (!IsEmpty())
    {
        MoveSelectionPositionAxis(selectionPosition_.x, direction.x);
        MoveSelectionPositionAxis(selectionPosition_.y, direction.y);
        ClampSelectionPosition(direction.y != 0);
    }
}

void SelectionGridModel::LocateSelection(const Math::Point2ui& position)
{
    selectionPosition_ = position;
    ClampSelectionPosition();
}

unsigned int SelectionGridModel::SelectionIndex() const
{
    return selectionPosition_.y * size_.width + selectionPosition_.x;
}

Math::Point2ui SelectionGridModel::PositionByIndex(unsigned int index) const
{
    if (!IsEmpty())
    {   
        /* Clamp index convert it to a selection position */
        index = std::min(index, numEntries_ - 1);
        return { index % size_.width, index / size_.width };
    }
    return { 0, 0 };
}

bool SelectionGridModel::IsEmpty() const
{
    return numEntries_ == 0;
}


/*
 * ======= Private: =======
 */

void SelectionGridModel::MoveSelectionPositionAxis(unsigned int& axis, int direction)
{
    if (direction < 0)
    {
        auto invDir = static_cast<unsigned int>(-direction);
        if (axis > invDir)
            axis -= invDir;
        else
            axis = 0;
    }
    else
        axis += static_cast<unsigned int>(direction);
}

void SelectionGridModel::ClampSelectionPosition(bool clampToBottom)
{
    if (!IsEmpty())
    {
        selectionPosition_.x = Math::Clamp(selectionPosition_.x, 0u, size_.width - 1);
        selectionPosition_.y = Math::Clamp(selectionPosition_.y, 0u, size_.height - 1);

        /* If position exceeded most right-down entry -> jump to end */
        if (selectionPosition_.y == size_.height - 1 && SelectionIndex() >= numEntries_)
        {
            if (!clampToBottom && selectionPosition_.y > 0)
                --selectionPosition_.y;
            else
                selectionPosition_.x = (numEntries_ - 1) % size_.width;
        }
    }
    else
        selectionPosition_ = { 0, 0 };
}


} // /namespace Utility

} // /namespace Fork



// ========================