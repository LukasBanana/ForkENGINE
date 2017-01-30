/*
 * Pixel shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PIXEL_SHADER_H__
#define __FORK_PIXEL_SHADER_H__


#include "Video/RenderSystem/Shader/Shader.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(PixelShader);

//! Pixel shader interface.
class FORK_EXPORT PixelShader : public Shader
{
    
    public:
        
        virtual ~PixelShader()
        {
        }

        PixelShader(const PixelShader&) = delete;
        PixelShader& operator = (const PixelShader&) = delete;

        Types Type() const override
        {
            return Types::Pixel;
        }

    protected:
        
        PixelShader()
        {
        }

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================