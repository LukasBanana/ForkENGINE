/*
 * Attribute iterator file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/BufferFormat/AttributeIterator.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/Exception/InvalidArgumentException.h"


namespace Fork
{

namespace Video
{


static void AssertStride(size_t stride, const char* functionName)
{
    if (stride == 0)
        throw InvalidArgumentException(functionName, "stride", "Attribute iteration stride must not be zero");
}

AttributeIterator::AttributeIterator(void* offset, size_t count, size_t stride) :
    begin_      { reinterpret_cast<char*>(offset) },
    end_        { begin_ + count * stride         },
    iterator_   { begin_                          },
    count_      { count                           },
    stride_     { stride                          }
{
    ASSERT_POINTER(offset);
    AssertStride(stride, __FUNCTION__);
}

AttributeConstIterator::AttributeConstIterator(const void* offset, size_t count, size_t stride) :
    begin_      { reinterpret_cast<const char*>(offset) },
    end_        { begin_ + count * stride               },
    iterator_   { begin_                                },
    count_      { count                                 },
    stride_     { stride                                }
{
    ASSERT_POINTER(offset);
    AssertStride(stride, __FUNCTION__);
}


} // /namespace Video

} // /namespace Fork



// ========================