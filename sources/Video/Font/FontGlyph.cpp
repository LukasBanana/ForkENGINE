/*
 * Font glyph file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/Font/FontGlyph.h"


namespace Fork
{

namespace Video
{


FontGlyph::FontGlyph(const Math::Recti& glyphRect) :
    rect{ glyphRect }
{
}
FontGlyph::FontGlyph(const Math::Recti& glyphRect, int glyphStartOffset, int glyphDrawnWidth, int glyphWhiteSpace) :
    rect        { glyphRect        },
    startOffset { glyphStartOffset },
    drawnWidth  { glyphDrawnWidth  },
    whiteSpace  { glyphWhiteSpace  }
{
}


} // /namespace Video

} // /namespace Fork



// ========================