/*
 * Editor default colors header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_DEFAULT_COLORS_H__
#define __FORK_EDITOR_DEFAULT_COLORS_H__


#include "Video/Core/ColorRGB.h"


namespace Fork
{

namespace Editor
{

namespace DefaultColors
{


static const Video::ColorRGBf xAxis = Video::ColorRGBub(250,  80,  10).Cast<float>();
static const Video::ColorRGBf yAxis = Video::ColorRGBub( 80, 230,  30).Cast<float>();
static const Video::ColorRGBf zAxis = Video::ColorRGBub( 20, 140, 250).Cast<float>();


} // /namespace DefaultColors

} // /namespace Editor

} // /namespace Fork


#endif



// ========================