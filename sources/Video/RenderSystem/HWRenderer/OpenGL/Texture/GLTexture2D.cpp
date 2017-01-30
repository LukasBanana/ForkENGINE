/*
 * OpenGL texture 2D file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLTexture2D.h"


namespace Fork
{

namespace Video
{


GLTexture2D::GLTexture2D() :
    GLTextureBase{ GL_TEXTURE_2D }
{
}


/*
 * ======= Private: =======
 */

void GLTexture2D::SetupArray(unsigned int arraySize)
{
    if (arraySize > 0)
    {
        this->arraySize = arraySize;
        SetTarget(GL_TEXTURE_2D_ARRAY);
    }
    else
    {
        this->arraySize = 0;
        SetTarget(GL_TEXTURE_2D);
    }
}

void GLTexture2D::SetupSize(const Texture2D::SizeType& size)
{
    this->size = size;
}


} // /namespace Video

} // /namespace Fork



// ========================