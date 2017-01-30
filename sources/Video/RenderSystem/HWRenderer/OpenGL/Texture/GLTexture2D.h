/*
 * OpenGL texture 2D header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_TEXTURE_2D_H__
#define __FORK_GL_TEXTURE_2D_H__


#include "Video/RenderSystem/Texture/Texture2D.h"
#include "GLTextureBase.h"


namespace Fork
{

namespace Video
{


class GLTexture2D : public Texture2D, public GLTextureBase
{
    
    public:
        
        GLTexture2D();

        GLTexture2D(const GLTexture2D&) = delete;
        GLTexture2D& operator = (const GLTexture2D&) = delete;
        
    private:
        
        friend class GLRenderSystem;

        void SetupArray(unsigned int arraySize);
        void SetupSize(const Texture2D::SizeType& size);

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================