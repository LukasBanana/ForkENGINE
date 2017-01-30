/*
 * Stride buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_STRIDE_BUFFER_H__
#define __FORK_STRIDE_BUFFER_H__


#include "Core/StaticConfig.h"
#include "Core/Exception/StrideMismatchException.h"
#include "Core/Exception/IndexOutOfBoundsException.h"

#include <vector>


namespace Fork
{


/**
The stride buffer is mainly used for vertex- and index buffers.
It contains a single 'std::vector<char>' container as raw-buffer,
but can access abitrary elements. It also alowes easier conversion
between vertex- and index formats.
*/
class StrideBuffer
{
    
    public:
        
        /* === Functions === */

        /**
        Sets the stride size. This must be greater than zero.
        \note This does not convert the buffer, thus you have to convert the buffer by
        yourself or change the stride only when the buffer is empty!
        */
        void SetStride(const size_t stride)
        {
            if (stride > 0)
                stride_ = stride;
        }

        //! Returns the stride size. By default 1.
        inline size_t GetStride() const
        {
            return stride_;
        }

        //! Returns the buffer size, in strides and not in bytes!
        inline size_t Size() const
        {
            return buffer_.size() / stride_;
        }
        //! Resizes the buffer, in strides and not in bytes!
        inline void Resize(const size_t size)
        {
            buffer_.resize(size * stride_);
        }

        //! Returns true if the buffer is empty.
        inline bool Empty() const
        {
            return buffer_.empty();
        }

        /**
        Returns a reference to the specified type at the specified index.
        \note Make sure the stride has the same size as sizeof(T)!
        */
        template <typename T> inline T& Get(const size_t index)
        {
            #ifdef FORK_DEBUG
            if (stride_ != sizeof(T))
                throw StrideMismatchException(__FUNCTION__, sizeof(T));
            if (index*stride_ >= buffer_.size())
                throw IndexOutOfBoundsException(__FUNCTION__, index);
            #endif
            return *reinterpret_cast<T*>(&buffer_[index*stride_]);
        }

        /**
        Returns a constant reference to the specified type at the specified index.
        \note Make sure the stride has the same size as sizeof(T)!
        */
        template <typename T> inline const T& Get(const size_t index) const
        {
            #ifdef FORK_DEBUG
            if (stride_ != sizeof(T))
                throw StrideMismatchException(__FUNCTION__, sizeof(T));
            if (index*stride_ >= buffer_.size())
                throw IndexOutOfBoundsException(__FUNCTION__, index);
            #endif
            return *reinterpret_cast<const T*>(&buffer_[index*stride_]);
        }

        template <typename T> inline void PushBack(const T& entry)
        {
            #ifdef FORK_DEBUG
            if (stride_ != sizeof(T))
                throw StrideMismatchException(__FUNCTION__, sizeof(T));
            #endif

            /* Resize buffer and copy entry into buffer */
            const auto prevSize = buffer_.size();
            buffer_.resize(buffer_.size() + stride_);

            *reinterpret_cast<T*>(&buffer_[prevSize]) = entry;
        }

        //! Retuns the constant raw pointer of this buffer.
        inline const char* Ptr() const
        {
            return buffer_.data();
        }
        //! Retuns the raw pointer of this buffer.
        inline char* Ptr()
        {
            return buffer_.data();
        }

    private:
        
        /* === Members === */

        size_t stride_ = 1;         //!< Buffer stride size (in bytes).
        std::vector<char> buffer_;  //!< Raw buffer.

};


} // /namespace Fork


#endif



// ========================