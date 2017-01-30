/*
 * Cursor header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PLATFORM_CURSOR_H__
#define __FORK_PLATFORM_CURSOR_H__


#include "Math/Core/Vector2.h"
#include "Math/Core/Rect.h"
#include "Core/Export.h"


namespace Fork
{

namespace Platform
{

class Frame;

namespace Cursor
{


//! Sets the global cursor position.
FORK_EXPORT void SetupPosition(const Math::Point2i& position);
//! Sets the cursor position relative to the specified frame and optionallity to its client area origin.
FORK_EXPORT void SetupPosition(const Math::Point2i& position, const Frame* frame, bool useClientAreaOrigin = true);

//! Returns the global cursor position.
FORK_EXPORT Math::Point2i Position();
//! Returns the cursor position relative to the specified frame and optionallity to its client area origin.
FORK_EXPORT Math::Point2i Position(const Frame* frame, bool useClientAreaOrigin = true);

/**
Clamps the cursor position to the specified rectangle (in screen coordinates).
To release the cursor position from the clamp region, call "Unclamp".
\see Unclamp
*/
FORK_EXPORT void Clamp(const Math::Recti& rect);
//! Disables the clamping region.
FORK_EXPORT void Unclamp();

//! Shows or hides the cursor.
FORK_EXPORT void Show(bool show = true);


} // /namespace Cursor

} // /namespace Platform

} // /namespace Fork


#endif



// ========================