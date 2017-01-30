/*
 * Storage buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_STORAGE_BUFFER_H__
#define __FORK_STORAGE_BUFFER_H__


#include "Video/RenderSystem/HardwareBuffer/StorageBufferException.h"
#include "Core/DeclPtr.h"
#include "Core/Export.h"

#include <vector>
#include <string>
#include <map>


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(StorageBuffer);

/**
Shader storage buffer interface.
Storage buffers can be used for read/write operations in a shader stage.
*/
class FORK_EXPORT StorageBuffer
{
    
    public:
        
        //! Storage buffer description structure.
        struct Description
        {
            std::string     name;           //!< Storage buffer name.
            unsigned int    index   = 0;    //!< Storage buffer index.
            size_t          size    = 0;    //!< Storage buffer size (in bytes).
        };

        typedef std::map<std::string, Description> DescriptionListType;
        typedef std::shared_ptr<Description> DescriptionPtr;

        virtual ~StorageBuffer();

        StorageBuffer(const StorageBuffer&) = delete;
        StorageBuffer& operator = (const StorageBuffer&) = delete;

        inline const Description& GetDesc() const
        {
            return desc_;
        }

    protected:
        
        StorageBuffer(const Description& desc);

    private:
        
        Description desc_; //!< Storage buffer description.

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================