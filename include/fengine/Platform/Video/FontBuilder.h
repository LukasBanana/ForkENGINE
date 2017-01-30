/*
 * Font builder header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_FONT_BUILDER_H__
#define __FORK_FONT_BUILDER_H__


#include "Core/Export.h"
#include "Video/Font/Font.h"
#include "Video/Font/FontGlyphSet.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Video/Image/Image.h"


namespace Fork
{

namespace Platform
{

namespace FontBuilder
{


/**
Builds the font glyph set described with the font-description.
\param[in] fontDesc Specifies the font description.
\param[out] fontGlyphSet Specifies the resulting font glyph set.
\see Font::Description
\see FontGlyph
\throws FontBuildException If building font failed.
\throws RenderContextException If there is no active render context.
\todo Remove 'RenderContextException' expection -> requires temporary HDC object for MS/Windows.
*/
FORK_EXPORT Video::ImageUBytePtr BuildFontAtlasImage(
    const Video::Font::Description& fontDesc,
    Video::FontGlyphSet& fontGlyphSet
);


} // /namespace FontBuilder

} // /namespace Platform

} // /namespace Fork


#endif



// ========================