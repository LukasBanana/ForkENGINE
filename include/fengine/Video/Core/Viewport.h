/*
 * Viewport header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_VIEWPORT_H__
#define __FORK_VIEWPORT_H__


#include "Math/Core/Vector2.h"
#include "Math/Core/Size2.h"
#include "Core/Export.h"


namespace Fork
{

namespace Video
{


/**
Viewport settings structure. This structure only stores the information about a viewport.
To update a graphics viewport, use the respective rendering class.
*/
struct FORK_EXPORT Viewport
{
    Viewport() = default;
    Viewport(const Math::Point2i& vpPosition, const Math::Size2i& vpSize);
    Viewport(const Math::Point2i& vpPosition, const Math::Size2i& vpSize, float vpMinDepth, float vpMaxDepth);

    //! Returns the aspact ratio. This is width/height;
    template <typename T> inline T AspectRatio() const
    {
        return static_cast<T>(size.width) / static_cast<T>(size.height);
    }

    //! Returns true if this is a valid viewport. This is the case, if (size.width > 0 && size.height > 0) holds true.
    inline bool IsValid() const
    {
        return size.width > 0 && size.height > 0;
    }

    Math::Point2i position; //!< Left-top position of the viewport (in screen space).
    Math::Size2i size;      //!< Size of the viewport (in screen space).

    float minDepth = 0.0f;  //!< Minimal depth range. Must be in the range [0.0 .. 1.0]. This may also be larger than "maxDepth". By default 0.0.
    float maxDepth = 1.0f;  //!< Maximal depth range. Must be in the range [0.0 .. 1.0]. This may also be smaller than "minDepth". By default 1.0.
};


/**
Compares the two viewports on equality.
For the floating pointer values the default == operator is used and not the "Math::Equal" function!
\see Math::Equal
*/
FORK_EXPORT bool operator == (const Viewport& a, const Viewport& b);
//! Compares the two viewports on unequality. This is equivalent to: !(a == b).
FORK_EXPORT bool operator != (const Viewport& a, const Viewport& b);


} // /namespace Video

} // /namespace Fork


#endif



// ========================