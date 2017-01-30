/*
 * HTML tags header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_HTML_TAGS_H__
#define __FORK_UTILITY_HTML_TAGS_H__


#include "Core/Export.h"
#include "Video/Core/ColorRGB.h"

#include <string>


namespace Fork
{

namespace Utility
{

namespace HTML
{


//! HTML "font" tag structure.
struct TagFont
{
    std::string         face;
    int                 size  { 0       };
    Video::ColorRGBub   color { 0, 0, 0 };
};


} // /namespace HTML

} // /namespace Utility

} // /namespace Fork


#endif



// ========================