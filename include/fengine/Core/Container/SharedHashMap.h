/*
 * Shared hash map header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SHARED_HASH_MAP_H__
#define __FORK_SHARED_HASH_MAP_H__


#include "Core/STLHelper.h"

#include <map>
#include <memory>


namespace Fork
{


/**
Shared memory hash map. This is a wrapper structure for the "std::map" class.
\tparam Key Specifies the hash key type.
\tparam Type Specifies the hash data type.
\tparam Compare Speciifes the compare type. By default std::less (like in the std::map class).
*/
template <class Key, class Type, class Compare = std::less<Key>> struct SharedHashMap
{
    using TypePtr = std::shared_ptr<Type>;

    /**
    Returns a shared pointer to the entry or
    null if the entry does not exist in this hash map.
    */
    TypePtr Find(const Key& key) const
    {
        auto it = hashMap.find(key);
        return it != hashMap.end() ? it->second : nullptr;
    }

    //! Removes only the first entry which matches the specified object.
    template <class BaseType> void Remove(const BaseType* object)
    {
        RemoveFromListIf(
            hashMap,
            [&object](const std::pair<Key, TypePtr>& entry)
            {
                return entry.second.get() == object;
            }
        );
    }

    std::map<Key, TypePtr, Compare> hashMap;
};


} // /namespace Fork


#endif



// ========================