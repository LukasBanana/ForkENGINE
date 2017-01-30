/*
 * Attribute iterator header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ATTRIBUTE_ITERATOR_H__
#define __FORK_ATTRIBUTE_ITERATOR_H__


#include "Core/Export.h"
#include "Video/BufferFormat/VertexFormat.h"
#include "Math/Core/Vector2.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/Vector4.h"


namespace Fork
{

namespace Video
{


/**
Common attribute (forward) iterator class.
This can be used to iterate over abitrary vertex buffers for instance.
\remarks Iteration usage example:
\code
for (AttributeIterator it{ &vertices[0].coord, vertices.size(), sizeof(Video::Simple3DVertex) }; it.HasNext(); it.Next())
{
    auto vertexCoord = AttributeConstIterator.Get<Math::Vector3f>();
    //...
}
\endcode
\see AttributeConstIterator
*/
class FORK_EXPORT AttributeIterator
{
    
    public:
        
        /**
        Initializes the attribute iterator.
        \param[in,out] offset Pointer to the buffer. This may also be a pointer with an offset to a desired attribute.
        \param[in] numAttributes Specifies the number of attribute.
        \param[in] stride Specifies the stride (in bytes) to the next element.
        \throws NullPointerException If 'offset' is empty
        \throws InvalidArgumentException If 'stride' is zero.
        \remarks Usage example:
        \code
        // Setup vertex buffer
        struct MyVertex { Math::Point3f coord; Math::Vector3f normal; Video::ColorRGBAf color; };
        std::vector<MyVertex> vertices(100);

        // Setup vertex iterator
        Video::AttributeIterator attribIt(&vertices[0].normal, vertices.size(), sizeof(MyVertex));

        // Iterator over all vertex normals
        while (attribIt.HasNext())
        {
            auto& normal = attribIt.Get<Math::Vector3f>();
            // Make use of 'normal' ...
            attribIt.Next();
        }
        \endcode
        \see VertexFormat::GetFormatSize
        */
        AttributeIterator(void* offset, size_t count, size_t stride);

        /**
        Returns true if there is a next element.
        \see Next
        */
        inline bool HasNext() const
        {
            return iterator_ < end_;
        }
        /**
        Moves the iterator to the next element,
        e.g. to the next vertex if this iterator is used with a vertex buffer.
        \see HasNext
        */
        inline void Next()
        {
            iterator_ += stride_;
        }

        //! Sets the iterator back to the beginning.
        inline void Reset()
        {
            iterator_ = begin_;
        }

        //! Returns the current index.
        inline size_t Index() const
        {
            return (iterator_ - begin_) / stride_;
        }

        /**
        Returns the current attribute.
        \remarks Usage example:
        \code
        Math::Vector3f& normal = attribIt.Get<Math::Vector3f>();
        \endcode
        \note No checks for iterator out of bounds are done here!
        */
        template <typename T> inline T& Get() const
        {
            return *reinterpret_cast<T*>(iterator_);
        }

        /**
        Returns the specified attribute.
        \remarks Usage example:
        \code
        for (size_t i = 0; i < attribIt.GetCount(); ++i)
            Math::Vector3f& normal = attribIt.Get<Math::Vector3f>(i);
        \endcode
        \note No checks for iterator out of bounds are done here!
        */
        template <typename T> inline T& Get(size_t index) const
        {
            return *reinterpret_cast<T*>(begin_ + index * stride_);
        }

        /**
        Returns the number of attributes.
        \remarks This is an exception to the coding conventions, because it is the
        count of arbitrary attributes, and not the 'number of vertices' for instance.
        */
        inline size_t GetCount() const
        {
            return count_;
        }

        //! Returns the stride (in bytes).
        inline size_t GetStride() const
        {
            return stride_;
        }

    private:
        
        char*   begin_      = nullptr;
        char*   end_        = nullptr;
        char*   iterator_   = nullptr;
        size_t  count_      = 0;
        size_t  stride_     = 0;

};


/**
Common attribute (forward) constant iterator class.
This can be used to iterate over abitrary vertex buffers for instance.
\remarks Iteration usage example:
\code
for (AttributeConstIterator it{ &vertices[0].coord, vertices.size(), sizeof(Video::Simple3DVertex) }; it.HasNext(); it.Next())
{
    auto vertexCoord = AttributeConstIterator.Get<Math::Vector3f>();
    //...
}
\endcode
\see AttributeIterator
*/
class FORK_EXPORT AttributeConstIterator
{
    
    public:
        
        //! \see AttributeIterator::AttributeIterator
        AttributeConstIterator(const void* offset, size_t count, size_t stride);

        /**
        Returns true if there is a next element.
        \see Next
        */
        inline bool HasNext() const
        {
            return iterator_ < end_;
        }
        /**
        Moves the iterator to the next element,
        e.g. to the next vertex if this iterator is used with a vertex buffer.
        \see HasNext
        */
        inline void Next()
        {
            iterator_ += stride_;
        }

        //! Sets the iterator back to the beginning.
        inline void Reset()
        {
            iterator_ = begin_;
        }

        //! Returns the current index.
        inline size_t Index() const
        {
            return (iterator_ - begin_) / stride_;
        }

        /**
        Returns the current attribute.
        \remarks Usage example:
        \code
        const auto& normal = attribIt.Get<Math::Vector3f>();
        \endcode
        \note No checks for iterator out of bounds are done here!
        */
        template <typename T> inline const T& Get() const
        {
            return *reinterpret_cast<const T*>(iterator_);
        }

        /**
        Returns the specified attribute.
        \remarks Usage example:
        \code
        for (size_t i = 0; i < attribIt.GetCount(); ++i)
            const Math::Vector3f& normal = attribIt.Get<Math::Vector3f>(i);
        \endcode
        \note No checks for iterator out of bounds are done here!
        */
        template <typename T> inline const T& Get(size_t index) const
        {
            return *reinterpret_cast<const T*>(begin_ + index * stride_);
        }

        /**
        Returns the number of attributes.
        \remarks This is an exception to the coding conventions, because it is the
        count of arbitrary attributes, and not the 'number of vertices' for instance.
        */
        inline size_t GetCount() const
        {
            return count_;
        }

        //! Returns the stride (in bytes).
        inline size_t GetStride() const
        {
            return stride_;
        }

    private:
        
        const char* begin_      = nullptr;
        const char* end_        = nullptr;
        const char* iterator_   = nullptr;
        size_t      count_      = 0;
        size_t      stride_     = 0;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================