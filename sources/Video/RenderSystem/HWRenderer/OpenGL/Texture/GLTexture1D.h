/*
 * OpenGL texture 1D header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_TEXTURE_1D_H__
#define __FORK_GL_TEXTURE_1D_H__


#include "Video/RenderSystem/Texture/Texture1D.h"
#include "GLTextureBase.h"


namespace Fork
{

namespace Video
{


class GLTexture1D : public Texture1D, public GLTextureBase
{
    
    public:
        
        GLTexture1D();

        GLTexture1D(const GLTexture1D&) = delete;
        GLTexture1D& operator = (const GLTexture1D&) = delete;
        
    private:
        
        friend class GLRenderSystem;

        void SetupArray(unsigned int arraySize);

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================