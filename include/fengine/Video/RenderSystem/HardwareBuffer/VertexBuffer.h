/*
 * Vertex buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_VERTEX_BUFFER_H__
#define __FORK_VERTEX_BUFFER_H__


#include "Video/BufferFormat/VertexFormat.h"
#include "Core/Export.h"

#include <memory>


namespace Fork
{

namespace Video
{


//! Vertex buffer interface.
class FORK_EXPORT VertexBuffer
{
    
    public:
        
        virtual ~VertexBuffer()
        {
        }

        /* === Interface === */

        virtual void SetupFormat(const VertexFormat& format) = 0;

        inline const VertexFormat& GetFormat() const
        {
            return format;
        }

    protected:
        
        friend class RenderSystem;

        VertexBuffer() = default;

        VertexFormat format;

};

typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;


} // /namespace Video

} // /namespace Fork


#endif



// ========================