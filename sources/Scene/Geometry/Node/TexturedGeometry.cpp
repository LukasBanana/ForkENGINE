/*
 * Textured geometry file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Node/TexturedGeometry.h"
#include "Core/AuxiliaryMacros.h"

#include <algorithm>


namespace Fork
{

namespace Scene
{


TexturedGeometry::~TexturedGeometry()
{
}

void TexturedGeometry::Visit(SceneVisitor* visitor)
{
    visitor->VisitTexturedGeometry(this);
}

Geometry::Types TexturedGeometry::Type() const
{
    return Geometry::Types::Textured;
}

void TexturedGeometry::ComputeBoundingVolume()
{
    if (actualGeometry)
    {
        actualGeometry->ComputeBoundingVolume();
        boundingVolume = actualGeometry->boundingVolume;
    }
}

bool TexturedGeometry::CompareTexturesSWO(const TexturedGeometry& other) const
{
    RETURN_COMPARE_MEMBER_SWO(textures.size())

    for (size_t i = 0, n = textures.size(); i < n; ++i)
    {
        /* Compare texture pointers */
        RETURN_COMPARE_MEMBER_SWO(textures[i])
    }

    return false;
}

bool TexturedGeometry::CompareTextures(const TexturedGeometry& other) const
{
    return
        textures.size() == other.textures.size() &&
        std::mismatch(textures.begin(), textures.end(), other.textures.begin()).first == textures.end();
}


} // /namespace Scene

} // /namespace Fork



// ========================