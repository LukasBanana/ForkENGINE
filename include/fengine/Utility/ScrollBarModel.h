/*
 * Scroll bar model header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_SCROLL_BAR_MODEL_H__
#define __FORK_UTILITY_SCROLL_BAR_MODEL_H__


#include "Core/Export.h"


namespace Fork
{

namespace Utility
{


/**
Base class for scroll bars in a user interface.
This class does not provide any data of the scroll bar position on the screen.
Only the scroll area and scroll position will be managed here.
*/
class FORK_EXPORT ScrollBarModel
{

    public:
        
        ScrollBarModel() = default;
        ScrollBarModel(float size, float areaSize, float scrollPos = 0.0f);
        virtual ~ScrollBarModel();

        /**
        Sets the visible scroll bar size.
        \param[in] size Specifies the new visible size. This will be clamped to the range [0.0 .. +inf).
        \see GetSize
        */
        void SetupSize(float size);
        /**
        Sets the virtual scroll bar size.
        \param[in] areaSize Specifies the new area (or rather virtual-) size. This will be clamped to the range [0.0 .. +inf).
        \remarks When the virtual scroll bar size is smaller than the visible size,
        the scroll bar can not be scrolled in any direction.
        \see GetAreaSize
        */
        void SetupAreaSize(float areaSize);
        /**
        Sets the minimal scroll bar size (not the entire size, i.e. not the minimal value for 'GetSize').
        \param[in] minBarSize Specifies the new minimal scroll bar size.
        By default 0.0. This will be clamped to the range [0.0 .. +inf).
        \remarks This affects 'BarPosition' and 'BarSize'.
        \see BarPosition
        \see BarSize
        */
        void SetupMinBarSize(float minBarSize);
        /**
        Sets the scroll position.
        \param[in] position Specifies the new scroll position. This must be in the range [0.0 .. 1.0].
        \remarks If you want to set the scroll position relative to the visible size
        (e.g. when the user scrolls the bar with the mouse) use 'SetupRelativeScrollPosition'.
        \see GetScrollPosition
        \see SetupRelativeScrollPosition
        */
        void SetupScrollPosition(float position);

        /**
        Scrolls the bar into the specified direction relative to the visible size.
        \remarks This function only exists for relative movement.
        */
        void Scroll(float direction);

        /**
        Sets the scroll position to the beginning. This is equivalent to:
        \code
        SetupScrollPosition(0.0f);
        \endcode
        */
        void ScrollBegin();
        /**
        Sets the scroll position to the end. This is equivalent to:
        \code
        SetupScrollPosition(1.0f);
        \endcode
        */
        void ScrollEnd();

        /**
        Scrolls one page forwards (or rather into end direction), relative to the visible size (see 'GetSize').
        \see ScrollEnd
        \see GetSize
        */
        void ScrollPageForwards();
        /**
        Scrolls one page backwards (or rather into beginning direction), relative to the visible size (see 'GetSize').
        \see ScrollBegin
        \see GetSize
        */
        void ScrollPageBackwards();

        //! Returns true if the scroll bar is at the beginning (GetScrollPosition() == 0.0).
        bool IsScrollBegin() const;
        //! Returns true if the scroll bar is at the end (GetScrollPosition() == 1.0).
        bool IsScrollEnd() const;
        
        /**
        Returns true if the scroll bar can be scrolled.
        Otherwise the area (or rather virtual-) size (see 'GetAreaSize')
        is smaller than or equal to the visible size (see 'GetSize').
        \see GetSize
        \see GetAreaSize
        */
        bool CanScroll() const;

        //! Returns the scroll bar size relative to the specified visual size.
        float BarSize(float visualSize) const;
        /**
        Returns the scroll bar size relative to the size (see 'GetSize').
        \see BarSize(float)
        \see GetSize
        */
        float BarSize() const;
        /**
        Returns the scroll bar position relative to the size (see 'GetSize').
        \param[in] isReverse Optional parameter to reverse the scroll position
        (1.0 - scrollPos) before the bar position is computed. By default false.
        \param[in] visualSize Specifies the visual scroll bar size.
        \see GetSize
        */
        float BarPosition(bool isReverse, float visualSize) const;
        /**
        Returns the scroll bar position relative to the size (see 'GetSize').
        \see BarPosition(bool)
        \see GetSize
        */
        float BarPosition(bool isReverse = false) const;

        /**
        Returns the scroll offset position. This is the offset relative to the non-visible area.
        If 'area size' is smaller than 'size', the return value is 0.0.
        \see GetAreaSize
        \see GetSize
        \see SetupScrollOffset
        */
        float ScrollOffset() const;
        /**
        Returns the scroll area. If "CanScroll" return false, the return value will be 0.0.
        \see CanScroll
        */
        float ScrollArea() const;

        /**
        Sets the new scroll position relative to the visible size (see 'GetSize').
        \param[in] relativePosition Specifies the new scroll position relative to the visible size.
        \see GetSize
        \see SetupScrollPosition
        */
        void SetupBarPosition(float relativePosition);

        //! Returns the visible scroll bar size.
        inline float GetSize() const
        {
            return size_;
        }
        //! Returns the virtual scroll bar area.
        inline float GetAreaSize() const
        {
            return areaSize_;
        }
        //! Returns the minimal scroll bar size.
        inline float GetMinBarSize() const
        {
            return minBarSize_;
        }
        //! Returns the absolute scroll bar position in the range [0.0 .. 1.0].
        inline float GetScrollPosition() const
        {
            return scrollPos_;
        }

    private:
        
        float size_         = 0.0f;
        float areaSize_     = 0.0f;
        float minBarSize_   = 0.0f;
        float scrollPos_    = 0.0f;

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================