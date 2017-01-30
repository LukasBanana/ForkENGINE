/*
 * Compute shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_COMPUTE_SHADER_H__
#define __FORK_COMPUTE_SHADER_H__


#include "Video/RenderSystem/Shader/Shader.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(ComputeShader);

//! Compute shader interface.
class FORK_EXPORT ComputeShader : public Shader
{
    
    public:
        
        virtual ~ComputeShader()
        {
        }

        ComputeShader(const ComputeShader&) = delete;
        ComputeShader& operator = (const ComputeShader&) = delete;

        Types Type() const override
        {
            return Types::Compute;
        }

    protected:
        
        ComputeShader()
        {
        }

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================