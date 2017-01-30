/*
 * Texture font file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/Font/TextureFont.h"


namespace Fork
{

namespace Video
{


TextureFont::TextureFont(
    const Font::Description& desc, const FontGlyphSet& glyphSet, const Texture2DPtr& texture) :
        Font    { desc, glyphSet },
        texture_{ texture        }
{
}

Font::Types TextureFont::Type() const
{
    return Types::Texture;
}


} // /namespace Video

} // /namespace Fork



// ========================