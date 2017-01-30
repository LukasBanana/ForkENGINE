/*
 * Tessellation control shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TESS_CONTROL_SHADER_H__
#define __FORK_TESS_CONTROL_SHADER_H__


#include "Video/RenderSystem/Shader/Shader.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(TessControlShader);

//! Tessellation control shader (also called "hull shader") interface.
class FORK_EXPORT TessControlShader : public Shader
{
    
    public:
        
        virtual ~TessControlShader()
        {
        }

        TessControlShader(const TessControlShader&) = delete;
        TessControlShader& operator = (const TessControlShader&) = delete;

        Types Type() const override
        {
            return Types::TessControl;
        }

    protected:
        
        TessControlShader()
        {
        }

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================