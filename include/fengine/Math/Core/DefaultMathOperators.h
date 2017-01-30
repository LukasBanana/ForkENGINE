/*
 * Default math operators header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_CORE_DEFAULT_OPERATORS_H__
#define __FORK_MATH_CORE_DEFAULT_OPERATORS_H__


#define DEFAULT_MATH_OPERATOR(c, OP, OPAssign)                              \
    template <typename T> c<T> operator OP (const c<T>& a, const c<T>& b)   \
    {                                                                       \
        auto result = a;                                                    \
        result OPAssign b;                                                  \
        return result;                                                      \
    }

#define DEFAULT_MATH_OPERATOR_DEFVAL(c, OP, OPAssign)                                                           \
    template <typename T, int defVal> c<T, defVal> operator OP (const c<T, defVal>& a, const c<T, defVal>& b)   \
    {                                                                                                           \
        auto result = a;                                                                                        \
        result OPAssign b;                                                                                      \
        return result;                                                                                          \
    }

#define DEFAULT_SCALAR_OPERATORS(c)                                             \
    template <typename T> c<T> operator * (const c<T>& vec, const T& scalar)    \
    {                                                                           \
        auto result = vec;                                                      \
        result *= scalar;                                                       \
        return result;                                                          \
    }                                                                           \
    template <typename T> c<T> operator * (const T& scalar, const c<T>& vec)    \
    {                                                                           \
        auto result = vec;                                                      \
        result *= scalar;                                                       \
        return result;                                                          \
    }                                                                           \
    template <typename T> c<T> operator / (const c<T>& vec, const T& scalar)    \
    {                                                                           \
        auto result = vec;                                                      \
        result /= scalar;                                                       \
        return result;                                                          \
    }

#define DEFAULT_COMPARE_OPERATORS(c)                                        \
    template <typename T> bool operator == (const c<T>& a, const c<T>& b)   \
    {                                                                       \
        return CompareVecEqual(a, b);                                       \
    }                                                                       \
    template <typename T> bool operator != (const c<T>& a, const c<T>& b)   \
    {                                                                       \
        return CompareVecNotEqual(a, b);                                    \
    }                                                                       \
    template <typename T> bool operator < (const c<T>& a, const c<T>& b)    \
    {                                                                       \
        return CompareVecLessThan(a, b);                                    \
    }                                                                       \
    template <typename T> bool operator > (const c<T>& a, const c<T>& b)    \
    {                                                                       \
        return CompareVecGreaterThan(a, b);                                 \
    }                                                                       \
    template <typename T> bool operator <= (const c<T>& a, const c<T>& b)   \
    {                                                                       \
        return CompareVecLessThanOrEqual(a, b);                             \
    }                                                                       \
    template <typename T> bool operator >= (const c<T>& a, const c<T>& b)   \
    {                                                                       \
        return CompareVecGreaterThanOrEqual(a, b);                          \
    }

#define DEFAULT_MATH_BASE_OPERATORS(c)  \
    DEFAULT_MATH_OPERATOR(c, +, +=)     \
    DEFAULT_MATH_OPERATOR(c, -, -=)     \
    DEFAULT_MATH_OPERATOR(c, *, *=)     \
    DEFAULT_MATH_OPERATOR(c, /, /=)


#endif



// ========================