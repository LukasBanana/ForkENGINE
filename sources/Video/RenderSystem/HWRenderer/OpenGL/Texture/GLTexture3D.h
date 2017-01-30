/*
 * OpenGL texture 3D header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_TEXTURE_3D_H__
#define __FORK_GL_TEXTURE_3D_H__


#include "Video/RenderSystem/Texture/Texture3D.h"
#include "GLTextureBase.h"


namespace Fork
{

namespace Video
{


class GLTexture3D : public Texture3D, public GLTextureBase
{
    
    public:
        
        GLTexture3D();

        GLTexture3D(const GLTexture3D&) = delete;
        GLTexture3D& operator = (const GLTexture3D&) = delete;
        
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================