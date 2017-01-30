/*
 * Storage buffer exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_STORAGE_BUFFER_EXCEPTION_H__
#define __FORK_STORAGE_BUFFER_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


namespace Video
{
    class StorageBuffer;
}


/**
Storage buffer exception.
\ingroup fork_exception
*/
class FORK_EXPORT StorageBufferException : public DefaultException
{
    
    public:
        
        StorageBufferException(const Video::StorageBuffer* storageBuffer, const std::string& errorDesc) :
            DefaultException
            {
                "StorageBufferException",
                "Description: " + errorDesc
            },
            storageBuffer_{ storageBuffer }
        {
        }

        inline const Video::StorageBuffer* GetConstBuffer() const
        {
            return storageBuffer_;
        }
        
    private:
        
        const Video::StorageBuffer* storageBuffer_ = nullptr;

};


} // /namespace Fork


#endif



// ========================