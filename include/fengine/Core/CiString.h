/*
 * Case insensitive string header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_CI_STRING_H__
#define __FORK_CI_STRING_H__


#include <string>
#include <cctype>


namespace Fork
{


/**
Case insensitive character traits structure.
\note This structure overwrites some functions of the 'std::char_traits' STL structure.
*/
template <typename T> struct CiCharTraits : public std::char_traits<T>
{
    static bool eq(T c1, T c2)
    {
        return std::toupper(c1) == std::toupper(c2);
    }

    static bool ne(T c1, T c2)
    {
        return std::toupper(c1) != std::toupper(c2);
    }

    static bool lt(T c1, T c2)
    {
        return std::toupper(c1) < std::toupper(c2);
    }

    static int compare(const T* s1, const T* s2, size_t n)
    {
        while (n-- != 0)
        {
            if (std::toupper(*s1) < std::toupper(*s2))
                return -1;
            if (std::toupper(*s1) > std::toupper(*s2))
                return 1;
            ++s1;
            ++s2;
        }
        return 0;
    }

    static const T* find(const T* s, int n, T a)
    {
        const auto ua(std::toupper(a));
        while (n-- > 0)
        {
            if (std::toupper(*s) == ua)
                return s;
            s++;
        }
        return nullptr;
    }
};


//! Case insensitive UTF-8 string.
typedef std::basic_string<char, CiCharTraits<char>> CiString;

//! Case insensitive UTF-16 string.
typedef std::basic_string<wchar_t, CiCharTraits<wchar_t>> CiWString;


} // /namespace Fork


#endif



// ========================