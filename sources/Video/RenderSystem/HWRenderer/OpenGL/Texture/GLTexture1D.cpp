/*
 * OpenGL texture 1D file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLTexture1D.h"


namespace Fork
{

namespace Video
{


GLTexture1D::GLTexture1D() :
    GLTextureBase{ GL_TEXTURE_1D }
{
}


/*
 * ======= Private: =======
 */

void GLTexture1D::SetupArray(unsigned int arraySize)
{
    if (arraySize > 0)
    {
        this->arraySize = arraySize;
        SetTarget(GL_TEXTURE_1D_ARRAY);
    }
    else
    {
        this->arraySize = 0;
        SetTarget(GL_TEXTURE_1D);
    }
}


} // /namespace Video

} // /namespace Fork



// ========================