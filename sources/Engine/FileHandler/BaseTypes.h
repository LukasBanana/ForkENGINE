/*
 * Engine format base types header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENGINE_FORMAT_BASE_TYPES_H__
#define __FORK_ENGINE_FORMAT_BASE_TYPES_H__


namespace Fork
{

namespace Engine
{

namespace Format
{


typedef char                Boolean;    //!< Boolean type (char).

typedef char                Char;       //!< UTF-8 character (char).
typedef wchar_t             WChar;      //!< UTF-16 character (wchar_t).

typedef signed char         Int8;       //!< 8-bit signed integer (char).
typedef unsigned char       UInt8;      //!< 8-bit unsigned integer (unsigned char).
typedef signed short        Int16;      //!< 16-bit signed integer (short).
typedef unsigned short      UInt16;     //!< 16-bit unsigned integer (unsigned short).
typedef signed int          Int32;      //!< 32-bit signed integer (int).
typedef unsigned int        UInt32;     //!< 32-bit unsigned integer (unsigned int).
typedef signed long long    Int64;      //!< 64-bit signed integer (long long).
typedef unsigned long long  UInt64;     //!< 64-bit unsigned integer (unsigned long long).

typedef float               Float32;    //!< 32-bit floating point (float).
typedef double              Float64;    //!< 64-bit floating point (double).


} // /namespace Format

} // /namespace Engine

} // /namespace Fork


#endif



// ========================