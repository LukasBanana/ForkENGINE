/*
 * OpenGL texture base header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_TEXTURE_BASE_H__
#define __FORK_GL_TEXTURE_BASE_H__


#include "../GLImport.h"


namespace Fork
{

namespace Video
{


class GLTextureBase
{
    
    public:
        
        virtual ~GLTextureBase();

        GLTextureBase(const GLTextureBase&) = delete;
        GLTextureBase& operator = (const GLTextureBase&) = delete;
        
        void Bind() const;
        void Unbind() const;

        void Bind(unsigned int layer) const;
        void Unbind(unsigned int layer) const;

        inline GLuint GetTextureID() const
        {
            return textureID_;
        }
        inline GLenum GetTarget() const
        {
            return target_;
        }

    protected:
        
        friend class GLRenderSystem;

        GLTextureBase(GLenum target);

        inline void SetTarget(GLenum target)
        {
            target_ = target;
        }

    private:
        
        GLuint textureID_ = 0;
        GLenum target_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================