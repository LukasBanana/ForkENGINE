/*
 * Invalid vertex format exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_INVALID_VERTEX_FORMAT_EXCEPTION_H__
#define __FORK_INVALID_VERTEX_FORMAT_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{

namespace Video
{
    class VertexFormat;
}


/**
Invalid vertex format exception.
\see Video::VertexFormat
\ingroup fork_exception
*/
class FORK_EXPORT InvalidVertexFormatException : public DefaultException
{
    
    public:
        
        InvalidVertexFormatException(const Video::VertexFormat* vertexFormat, const std::string& errorDesc) :
            DefaultException
            {
                "InvalidVertexFormatException",
                "Description: " + errorDesc
            },
            vertexFormat_(vertexFormat)
        {
        }

        //! Returns the vertex format. This may also be null!
        inline const Video::VertexFormat* GetVertexFormat() const
        {
            return vertexFormat_;
        }

    private:
        
        const Video::VertexFormat* vertexFormat_;

};


} // /namespace Fork


#endif



// ========================