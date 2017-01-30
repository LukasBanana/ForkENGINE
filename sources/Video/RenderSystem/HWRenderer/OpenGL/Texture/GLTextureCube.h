/*
 * OpenGL texture cube header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_TEXTURE_CUBE_H__
#define __FORK_GL_TEXTURE_CUBE_H__


#include "Video/RenderSystem/Texture/TextureCube.h"
#include "GLTextureBase.h"


namespace Fork
{

namespace Video
{


class GLTextureCube : public TextureCube, public GLTextureBase
{
    
    public:
        
        GLTextureCube();

        GLTextureCube(const GLTextureCube&) = delete;
        GLTextureCube& operator = (const GLTextureCube&) = delete;
        
    private:
        
        friend class GLRenderSystem;

        void SetupArray(unsigned int arraySize);
        void SetupSize(const TextureCube::SizeType& size);

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================