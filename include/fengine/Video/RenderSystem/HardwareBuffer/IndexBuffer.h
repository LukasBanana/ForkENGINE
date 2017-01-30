/*
 * Index buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_INDEX_BUFFER_H__
#define __FORK_INDEX_BUFFER_H__


#include "Video/BufferFormat/IndexFormat.h"
#include "Core/Export.h"

#include <memory>


namespace Fork
{

namespace Video
{


//! Index buffer interface.
class FORK_EXPORT IndexBuffer
{
    
    public:
        
        virtual ~IndexBuffer()
        {
        }

        /* === Interface === */

        inline const IndexFormat& GetFormat() const
        {
            return format;
        }

    protected:
        
        friend class RenderSystem;

        IndexBuffer()
        {
        }

        IndexFormat format;

};

typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;


} // /namespace Video

} // /namespace Fork


#endif



// ========================