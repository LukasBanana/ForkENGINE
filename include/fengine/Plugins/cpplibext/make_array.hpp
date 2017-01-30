/*
 * make_array.hpp file
 * 
 * Copyright (C) 2014 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef _CPPLIBEXT_MAKE_ARRAY_H_
#define _CPPLIBEXT_MAKE_ARRAY_H_


#include <memory>


namespace ext
{


/**
Makes a new shared_ptr of type 'T[size]' and sets a custom deleter to allow array pointers.
\tparam T Specifies the pointer type.
\param[in] size Specifies the array size.
*/
template <class T> std::shared_ptr<T> make_shared_array(const size_t size)
{
    /* Allocate shared memory manually and set custom deleter to allow array pointers */
    return std::shared_ptr<T>(
        new T[size],
        [](T* ptr)
        {
            delete [] ptr;
        }
    );
}


} // /namespace ext


#endif


