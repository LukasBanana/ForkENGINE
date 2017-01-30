/*
 * GUI base types header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_BASE_TYPES_H__
#define __FORK_GUI_BASE_TYPES_H__


#include "Math/Core/Vector2.h"
#include "Math/Core/Size2.h"
#include "Math/Core/Rect.h"
#include "Core/Container/Identifiable.h"
#include "Video/Core/ColorRGBA.h"
#include "Math/Common/StaticBezier.h"
#include "IO/InputDevice/KeyCodes.h"
#include "IO/InputDevice/MouseKeyCodes.h"

#include <string>


namespace Fork
{

namespace GUI
{


//! Base GUI type for ID numbers.
typedef Identifiable::IDType IDType;

//! Base GUI type for trigger ID numbers.
typedef unsigned int TriggerIDType;

//! Base GUI type for 2D points.
typedef Math::Point2i Point;

//! Base GUI type for 2D sizes.
typedef Math::Size2i Size;

//! Base GUI type for rectangles.
typedef Math::Recti Rect;

//! Base GUI type for colors. This is RGBA color (32 bit).
typedef Video::ColorRGBAub Color;

//! Base GUI type for strings. This is a UTF-16 string.
typedef std::wstring String;

//! Base GUI type for bezier curves.
typedef Math::StaticBezier<Point, 4> BezierCurve;

//! Key code type for the GUI.
typedef IO::KeyCodes KeyCodes;

//! Mouse key code type for the GUI.
typedef IO::MouseKeyCodes MouseKeyCodes;


} // /namespace GUI

} // /namespace Fork


#endif



// ========================