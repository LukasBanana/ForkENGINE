/*
 * OpenGL query header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_RENDERSYSTEM_QUERY_H__
#define __FORK_GL_RENDERSYSTEM_QUERY_H__


#include "Video/RenderSystem/Query.h"
#include "GLImport.h"


namespace Fork
{

namespace Video
{


//! OpenGL query implementation.
class GLQuery : public Query
{
    
    public:
        
        GLQuery(const Types type);
        ~GLQuery();

        GLQuery(const GLQuery&) = delete;
        GLQuery& operator = (const GLQuery&) = delete;

        inline GLuint GetObjectID() const
        {
            return objectID_;
        }

        inline GLenum GetTarget() const
        {
            return target_;
        }

    private:
        
        GLuint objectID_    = 0;
        GLenum target_      = 0;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================