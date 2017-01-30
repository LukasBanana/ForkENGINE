/*
 * Vertex shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_VERTEX_SHADER_H__
#define __FORK_VERTEX_SHADER_H__


#include "Video/RenderSystem/Shader/Shader.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(VertexShader);

//! Vertex shader interface.
class FORK_EXPORT VertexShader : public Shader
{
    
    public:
        
        virtual ~VertexShader()
        {
        }

        VertexShader(const VertexShader&) = delete;
        VertexShader& operator = (const VertexShader&) = delete;

        Types Type() const override
        {
            return Types::Vertex;
        }

    protected:
        
        VertexShader()
        {
        }

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================