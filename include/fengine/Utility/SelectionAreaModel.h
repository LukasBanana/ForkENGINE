/*
 * Selection area model header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_SELECTION_AREA_MODEL_H__
#define __FORK_UTILITY_SELECTION_AREA_MODEL_H__


#include "Core/Export.h"
#include "Math/Core/Rect.h"


namespace Fork
{

namespace Utility
{


/**
Base class for selection areas in a user interface.
This is used in the editor when the user clicks and holds the left mouse button,
to drag the selection area over the editor view (or asset brower).
*/
class FORK_EXPORT SelectionAreaModel
{

    public:
        
        virtual ~SelectionAreaModel();

        /* === Functions === */

        //! Activates dragging.
        void StartDrag();
        //! Deactivates dragging.
        void StopDrag();

        /**
        Locates the left-top or right-bottom position,
        depending on the current state.
        \see 
        */
        void Locate(const Math::Point2f& position);

        //! Returns the selection area rectangle.
        Math::Rectf AreaRect() const;

        /**
        Returns true if the selection area is currently active (i.e. it is dragged).
        Call 'StartDrag' to activate the selection area.
        \see StartDrag.
        */
        inline bool IsActive() const
        {
            return isActive_;
        }

        /* === Operators === */

        /**
        Returns the selection area rectangle (shortcut for 'GetAreaRect').
        \see GetAreaRect
        */
        Math::Rectf operator () () const;

        /**
        Returns true if the selection area is currently active (shortcut for 'IsActive').
        \see IsActive
        */
        operator bool() const;

    private:
        
        bool        isActive_ = false;
        Math::Rectf areaRect_;

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================