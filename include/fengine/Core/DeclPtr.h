/*
 * Declare shared pointer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_DECL_PTR_H__
#define __FORK_DECL_PTR_H__


#include <memory>

//! Declares a type alias "n##Ptr" of shared_ptr<n> and "n##ConstPtr" of shared_ptr<const n> for the type 'n'.
#define DECL_SHR_PTR(n)                             \
    class n;                                        \
    typedef std::shared_ptr<n> n##Ptr;              \
    typedef std::shared_ptr<const n> n##ConstPtr;

//! Declares a type alias "n##Ptr" of unique_ptr<n> for the type 'n'.
#define DECL_UNQ_PTR(n)                 \
    class n;                            \
    typedef std::unique_ptr<n> n##Ptr;  \


#endif



// ========================