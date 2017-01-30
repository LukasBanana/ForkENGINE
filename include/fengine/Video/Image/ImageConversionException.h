/*
 * Image conversion exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IMAGE_CONVERSION_EXCEPTION_H__
#define __FORK_IMAGE_CONVERSION_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"
#include "Core/StringModifier.h"
#include "Math/Core/Size3.h"


namespace Fork
{


/**
Image conversion exception.
\ingroup fork_exception
*/
class FORK_EXPORT ImageConversionException : public DefaultException
{
    
    public:
        
        ImageConversionException(const std::string& procName, const Math::Size3st& size, size_t components) :
            DefaultException
            {
                "ImageConversionException",
                "Function: " + procName +
                    ", Width: " + ToStr(size.width) + ", Height: " + ToStr(size.height) +
                    ", Depth: " + ToStr(size.depth) + ", Color Components: " + ToStr(components)
            }
        {
        }

};


} // /namespace Fork


#endif



// ========================