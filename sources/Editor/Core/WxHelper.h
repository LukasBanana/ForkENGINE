/*
 * Editor wxWidgets helper header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_WXWIDGETS_HELPER_H__
#define __FORK_EDITOR_WXWIDGETS_HELPER_H__


#include "Video/Core/ColorRGB.h"
#include "Video/Core/ColorRGBA.h"

#include <wx/colour.h>


namespace Fork
{

namespace Editor
{


/**
Allocates a new wxWidgets object.
\note This is just a wrapper function for the "new" operator,
since wxWidgets implements its own reference counted mechanism.
*/
template <class T> T* MakeWxObject()
{
    return new T();
}

/**
Allocates a new wxWidgets object.
\note This is just a wrapper function for the "new" operator,
since wxWidgets implements its own reference counted mechanism.
*/
template <class T, typename... Args> T* MakeWxObject(Args&&... args)
{
    return new T(std::forward<Args>(args)...);
}

/**
Deallocates the specified wxWidgets object.
\note This is just a wrapper function for the "delete" operator,
since wxWigdets implements its own reference counted mechanism.
*/
template <class T> void FreeWxObject(T object)
{
    delete object;
}

//! Converts wxColour to Video::ColorRGB<T>.
template <typename T> Video::ColorRGB<T> WxColorToRGB(const wxColour& color)
{
    return Video::ColorRGBub(color.Red(), color.Green(), color.Blue()).Cast<T>();
}

//! Converts wxColour to Video::ColorRGBA<T>.
template <typename T> Video::ColorRGBA<T> WxColorToRGBA(const wxColour& color)
{
    return Video::ColorRGBAub(color.Red(), color.Green(), color.Blue(), color.Alpha()).Cast<T>();
}

//! Converts Video::ColorRGB<T> to wxColour.
template <typename T> wxColour RGBToWxColor(const Video::ColorRGB<T>& color)
{
    auto ubColor = color.Cast<unsigned char>();
    return wxColour(ubColor.r, ubColor.g, ubColor.b);
}

//! Converts the wxColour to Video::ColorRGBA<T>.
template <typename T> wxColour RGBAToWxColor(const Video::ColorRGBA<T>& color)
{
    auto ubColor = color.Cast<unsigned char>();
    return wxColour(ubColor.r, ubColor.g, ubColor.b, ubColor.a);
}


} // /namespace Editor

} // /namespace Fork


#endif



// ========================