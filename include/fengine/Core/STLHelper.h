/*
 * STL helper functions header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_STL_HELPER_H__
#define __FORK_STL_HELPER_H__


#include <vector>
#include <iterator>
#include <memory>
#include <random>
#include <algorithm>
#include <type_traits>


namespace Fork
{


/**
Initializes the specified data block zero zeros.
This is a templated version of 'memset(&data, 0, sizeof(data))' or 'ZeroMemory' (in Win32 API).
\tparam T Specifies the data type. This may be a primitive data type (such as int, float etc.)
or a structure or class. Pointers are not allowed!
\param[out] data Resulting data block which is to be initialized.
*/
template <typename T> void InitRawMemory(T& data)
{
    static_assert(!std::is_pointer<T>::value, "\"" __FUNCTION__ "\" does not allow pointer types");
    static_assert(std::is_pod<T>::value, "\"" __FUNCTION__ "\" does only allow plain-old-data (POD)");
    memset(&data, 0, sizeof(T));
}

/**
Copy raw memory.
\tparam T Specifies the data type. Pointers are not allowed!
*/
template <typename Dest, typename Src> void CopyRawMemory(Dest& dest, const Src& src)
{
    static_assert(!std::is_pointer<Dest>::value && !std::is_pointer<Src>::value, "\"" __FUNCTION__ "\" does not allow pointer types");
    static_assert(sizeof(Dest) >= sizeof(Src), "Destination buffer is too small in \"" __FUNCTION__ "\"");
    static_assert(std::is_pod<Dest>::value && std::is_pod<Src>::value, "\"" __FUNCTION__ "\" does only allow plain-old-data (POD)");
    memcpy(&dest, &src, sizeof(Src));
}

//! Returns the forward iterator of the specified reverse iterator.
template <class ReverseIterator> typename ReverseIterator::iterator_type MakeForward(ReverseIterator rit)
{
    return --(rit.base());
}

/**
Returns a pseudo random number in the specified range [min .. max].
\remarks The default pseudo random number generator is a uniform integral distribution (std::uniform_int_distrubution).
For float types ('float' and 'double') this will be a uniform real distribution (std::uniform_real_distribution).
*/
template <typename T> inline T RandomUniformDistribution(const T& min, const T& max)
{
    /* Simple uniform integral random number generator */
    std::random_device randDevice;
    std::mt19937 randGen(randDevice());
    std::uniform_int_distribution<T> randDistr(min, max);
    return randDistr(randGen);
}

//! Template specialization for the "RandomUniformDistribution<float>" function.
template <> inline float RandomUniformDistribution<float>(const float& min, const float& max)
{
    /* Simple uniform real random number generator */
    std::random_device randDevice;
    std::mt19937 randGen(randDevice());
    std::uniform_real_distribution<float> randDistr(min, max);
    return randDistr(randGen);
}

//! Template specialization for the "RandomUniformDistribution<double>" function.
template <> inline double RandomUniformDistribution<double>(const double& min, const double& max)
{
    /* Simple uniform real random number generator */
    std::random_device randDevice;
    std::mt19937 randGen(randDevice());
    std::uniform_real_distribution<double> randDistr(min, max);
    return randDistr(randGen);
}

//! Removes only the first entry that equals the specified value from the contianer.
template <class Container, class T> void RemoveFromList(Container& container, const T& value)
{
    auto it = std::find(container.begin(), container.end(), value);
    if (it != container.end())
        container.erase(it);
}

//! Removes only the first all entry from the contianer for which the predicate is true.
template <class Container, class UnaryPredicate> void RemoveFromListIf(Container& container, UnaryPredicate predicate)
{
    auto it = std::find_if(container.begin(), container.end(), predicate);
    if (it != container.end())
        container.erase(it);
}

//! Removes all entries that equals the specified value from the contianer.
template <class Container, class T> void RemoveAllFromList(Container& container, const T& value)
{
    /*
    'std::remove' will re-arange the container, then 'erase' will
    remove all elements, which have been moved to the end of the list.
    */
    container.erase(
        std::remove(container.begin(), container.end(), value),
        container.end()
    );
}

//! Removes all entries from the contianer for which the predicate is true.
template <class Container, class UnaryPredicate> void RemoveAllFromListIf(Container& container, UnaryPredicate predicate)
{
    /*
    'std::remove_if' will re-arange the container, then 'erase' will
    remove all elements, which have been moved to the end of the list.
    */
    container.erase(
        std::remove_if(container.begin(), container.end(), predicate),
        container.end()
    );
}

/**
Helper function to apply another function for each element in the entire specified container.
\remarks This is equivalent to:
\code
std::for_each(container.begin(), container.end(), function);
\endcode
*/
template <class Container, class Function> void ForEach(Container& container, Function function)
{
    std::for_each(container.begin(), container.end(), function);
}


} // /namespace Fork


#endif



// ========================