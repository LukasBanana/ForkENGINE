/*
 * Tessellation evaluation shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TESS_EVALUATION_SHADER_H__
#define __FORK_TESS_EVALUATION_SHADER_H__


#include "Video/RenderSystem/Shader/Shader.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(TessEvaluationShader);

//! Tessellation evaluation shader (also called "domain shader") interface.
class FORK_EXPORT TessEvaluationShader : public Shader
{
    
    public:
        
        virtual ~TessEvaluationShader()
        {
        }

        TessEvaluationShader(const TessEvaluationShader&) = delete;
        TessEvaluationShader& operator = (const TessEvaluationShader&) = delete;

        Types Type() const override
        {
            return Types::TessEvaluation;
        }

    protected:
        
        TessEvaluationShader()
        {
        }

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================