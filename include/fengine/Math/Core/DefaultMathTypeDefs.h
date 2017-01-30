/*
 * Default math typedefs header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_CORE_DEFAULT_TYPEDEFS_H__
#define __FORK_MATH_CORE_DEFAULT_TYPEDEFS_H__


#define DEFAULT_MATH_TYPEDEFS(c)        \
    typedef c<bool          > c##b;     \
    typedef c<int           > c##i;     \
    typedef c<unsigned int  > c##ui;    \
    typedef c<float         > c##f;     \
    typedef c<double        > c##d;     \
    typedef c<size_t        > c##st;

#define USE_BASE_MATH_CLASS(c)  \
    using Math::c;              \
    using Math::c##b;           \
    using Math::c##i;           \
    using Math::c##ui;          \
    using Math::c##f;           \
    using Math::c##d;           \
    using Math::c##st;


#endif



// ========================