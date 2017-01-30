/*
 * OpenGL texture cube file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLTextureCube.h"


namespace Fork
{

namespace Video
{


GLTextureCube::GLTextureCube() :
    GLTextureBase{ GL_TEXTURE_CUBE_MAP }
{
}


/*
 * ======= Private: =======
 */

void GLTextureCube::SetupArray(unsigned int arraySize)
{
    if (arraySize > 0)
    {
        this->arraySize = arraySize;
        SetTarget(GL_TEXTURE_CUBE_MAP_ARRAY);
    }
    else
    {
        this->arraySize = 0;
        SetTarget(GL_TEXTURE_CUBE_MAP);
    }
}

void GLTextureCube::SetupSize(const TextureCube::SizeType& size)
{
    this->size = size;
}


} // /namespace Video

} // /namespace Fork



// ========================