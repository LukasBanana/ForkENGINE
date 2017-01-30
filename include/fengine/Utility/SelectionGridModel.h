/*
 * Selection grid model header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_SELECTION_GRID_MODEL_H__
#define __FORK_UTILITY_SELECTION_GRID_MODEL_H__


#include "Core/Export.h"
#include "Math/Core/Size2.h"
#include "Math/Core/Vector2.h"


namespace Fork
{

namespace Utility
{


/**
Base class for selection grids in a user interface.
This is used in the editor for the asset browser,
where each asset preview in a grid can be selected.
*/
class FORK_EXPORT SelectionGridModel
{

    public:
        
        virtual ~SelectionGridModel();

        /* === Functions === */

        /**
        Resizes the grid and moves the selection so that is selects the same inde.x
        \param[in] numEntries Specifies the number of entries for this grid model.
        \param[in] numEntriesPerRow Specifies the maximal number of entries for each row. This will be clamped to [1 .. +inf).
        \see SelectionIndex
        */
        void ResizeGrid(unsigned int numEntries, unsigned int numEntriesPerRow);

        //! Moves the selection position into the specified 2D direction.
        void MoveSelection(const Math::Vector2i& direction);
        //! Sets the new selection position.
        void LocateSelection(const Math::Point2ui& position);

        //! Returns the selection index by the current position (index := y * width + x).
        unsigned int SelectionIndex() const;
        /**
        Computes the selection position by the specified index.
        \param[in] index Specifies the index which is to be converted
        to a selection position. This will be clamped to [0 .. numEntries - 1].
        \return The converted selection position. This is in the range { [0 .. width - 1], [0 .. height - 1] }.
        \see GetSelectionPosition
        \see SelectionIndex
        */
        Math::Point2ui PositionByIndex(unsigned int index) const;

        //! Returns true if this selection grid is empty.
        bool IsEmpty() const;

        //! Returns the 2D grid size.
        inline const Math::Size2ui& GetGridSize() const
        {
            return size_;
        }
        /**
        Returns the 2D selection position.
        \remarks X and Y coordinates are always in the range { [0 .. width - 1], [0 .. height - 1] }
        or the position is { 0, 0 } if width or height is 0.
        */
        inline const Math::Point2ui& GetSelectionPosition() const
        {
            return selectionPosition_;
        }
        //! Returns the number of entries for this grid model.
        inline unsigned int GetNumEntries() const
        {
            return numEntries_;
        }

    private:
        
        void MoveSelectionPositionAxis(unsigned int& axis, int direction);
        void ClampSelectionPosition(bool clampToBottom = true);

        Math::Size2ui   size_;
        Math::Point2ui  selectionPosition_;
        unsigned int    numEntries_ = 0;

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================