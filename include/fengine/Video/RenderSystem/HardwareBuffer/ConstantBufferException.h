/*
 * Constant buffer exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_CONSTANT_BUFFER_EXCEPTION_H__
#define __FORK_CONSTANT_BUFFER_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


namespace Video
{
    class ConstantBuffer;
}


/**
Constant buffer exception.
\ingroup fork_exception
*/
class FORK_EXPORT ConstantBufferException : public DefaultException
{
    
    public:
        
        ConstantBufferException(const Video::ConstantBuffer* constBuffer, const std::string& errorDesc) :
            DefaultException
            {
                "ConstantBufferException",
                "Description: " + errorDesc
            },
            constBuffer_{ constBuffer }
        {
        }

        inline const Video::ConstantBuffer* GetConstBuffer() const
        {
            return constBuffer_;
        }
        
    private:
        
        const Video::ConstantBuffer* constBuffer_ = nullptr;

};


} // /namespace Fork


#endif



// ========================