/*
 * Auxiliary macros header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_AUX_MACROS_H__
#define __FORK_AUX_MACROS_H__


/* --- DO NOT INCLUDE THIS GLOBALLY --- */

/**
Safe function call for nullable pointers:
\code
SAFE_CALL(obj)->Proc();

// equivalent to ...

if (obj) (obj)->Proc();
\endcode
*/
#define SAFE_CALL(ptr) if (ptr) (ptr)

/**
Returns true if 'lhs' is less than 'rhs' or
returns false if 'rhs' is greater than 'rhs'.
*/
#define RETURN_COMPARE_SWO(lhs, rhs)    \
    if ((lhs) < (rhs)) return true;     \
    if ((lhs) > (rhs)) return false;

//! Uses 'RETURN_COMPARE_SWO' to compare 'member' with 'other.member'.
#define RETURN_COMPARE_MEMBER_SWO(member) \
    RETURN_COMPARE_SWO(member, other.member)

//! Converts the specified string (must have 4 characters!) to a 32-bit hex value.
#define HEX_FROM_STRING(str) (*reinterpret_cast<const long*>(str))


#endif



// ========================